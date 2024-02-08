/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2021 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include "AppTask.h"

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/util/attribute-table.h>
#if (MW320_FEATURE_BIND==1)
    #include <app/clusters/bindings/BindingManager.h>
#endif // MW320_FEATURE_BIND
#include <platform/nxp/mw320/ConnectivityUtils.h>


/* platform specific */
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_pinmux.h"


#include <wm_os.h>
extern "C" {
#include "boot_flags.h"
#include "cli.h"
#include "dhcp-server.h"
#include "iperf.h"
#include "mflash_drv.h"
#include "network_flash_storage.h"
#include "partition.h"
#include "wifi_ping.h"
#include "wlan.h"
#include "wm_net.h"
}
#include "fsl_aes.h"
#include "lpm.h"
#include "mw320_ota.h"
#include "mw320-binding-handler.h"


#define SSID_FNAME "ssid_fname"
#define PSK_FNAME "psk_fname"
#define DEV_OP_STATE_FNAME "ops_fname"

#define RUN_RST_LT_DELAY 10
#define APP_AES AES
#define CONNECTION_INFO_FILENAME "connection_info.dat"

volatile int g_ButtonPress = 0;
bool need2sync_sw_attr     = false;

using namespace chip;
using namespace chip::app::Clusters;
using namespace chip::DeviceLayer;

static SemaphoreHandle_t aesLock;
static void rst_args_lt(System::Layer * aSystemLayer, void * aAppState);
static void gpio_init(void);
static void mcuInitPower(void);
//=============================================================================
// Light behaior while resetting the saved arguments
//
static void rst_args_lt(System::Layer * aSystemLayer, void * aAppState)
{
    // PRINTF("%s(), Turn on lights \r\n", __FUNCTION__);
    led_on_off(led_amber, true);
    led_on_off(led_yellow, true);
    // sleep 3 second
    // PRINTF("%s(), sleep 3 seconds \r\n", __FUNCTION__);
    os_thread_sleep(os_msec_to_ticks(3000));
    // PRINTF("%s(), Turn off lights \r\n", __FUNCTION__);
    led_on_off(led_amber, false);
    led_on_off(led_yellow, false);
    ::mw320_dev_reset(1000);
    return;
}

static void mcuInitPower(void)
{
    lpm_config_t config = {
        /* System PM2/PM3 less than 50 ms will be skipped. */
        .threshold = 50U,
        /* SFLL config and  RC32M setup takes approx 14 ms. */
        .latency          = 15U,
        .enableWakeupPin0 = true,
        .enableWakeupPin1 = true,
        .handler          = NULL,
    };

    LPM_Init(&config);
}

static status_t APP_AES_Lock(void)
{
    if (pdTRUE == xSemaphoreTakeRecursive(aesLock, portMAX_DELAY))
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

static void APP_AES_Unlock(void)
{
    xSemaphoreGiveRecursive(aesLock);
}

void led_on_off(led_id_t lt_id, bool is_on)
{
    GPIO_Type * pgpio;
    uint32_t gpio_pin;

    // Configure the GPIO / PIN
    switch (lt_id)
    {
    case led_amber:
        pgpio    = BOARD_LED_AMBER_GPIO;
        gpio_pin = BOARD_LED_AMBER_GPIO_PIN;
        break;
    case led_yellow:
    default: // Note: led_yellow as default
        pgpio    = BOARD_LED_YELLOW_GPIO;
        gpio_pin = BOARD_LED_YELLOW_GPIO_PIN;
    }

    // Do on/off the LED
#ifdef MW320_EVA_BOARD
    if (is_on == true)
    {
        // PRINTF("led on\r\n");
        GPIO_PortClear(pgpio, GPIO_PORT(gpio_pin), 1u << GPIO_PORT_PIN(gpio_pin));
    }
    else
    {
        // PRINTF("led off\r\n");
        GPIO_PortSet(pgpio, GPIO_PORT(gpio_pin), 1u << GPIO_PORT_PIN(gpio_pin));
    }
#else
    if (is_on == true)
    {
        // PRINTF("led on\r\n");
        GPIO_PortSet(pgpio, GPIO_PORT(gpio_pin), 1u << GPIO_PORT_PIN(gpio_pin));
    }
    else
    {
        // PRINTF("led off\r\n");
        GPIO_PortClear(pgpio, GPIO_PORT(gpio_pin), 1u << GPIO_PORT_PIN(gpio_pin));
    }

#endif // MW320_EVA_BOARD
    return;
}

void sw2_handle(bool frm_clk)
{
    static uint8_t click_cnt = 0;
    static uint8_t run_times = 0;

    if (frm_clk == true)
    {
        // Called while user clicks the button
        click_cnt++;
        ChipLogProgress(NotSpecified, " (%d times)", click_cnt);
        return;
    }
    // Called regularlly from a thread every 500ms
    run_times++;
    if (click_cnt > 4)
    {
        // More than 4 clicks within the last second => erase the saved parameters
        PRINTF("--> enough clicks (%d times) => resetting the saved parameters \r\n", click_cnt);
        ::erase_all_params();
        DeviceLayer::SystemLayer().StartTimer(System::Clock::Milliseconds32(RUN_RST_LT_DELAY), rst_args_lt, nullptr);
        click_cnt = 0;
        // Reboot the device
        ::mw320_dev_reset(1000);
    }
    if (run_times >= 2)
    {
        // Called twice with gap==500ms
        click_cnt = 0;
        run_times = 0;
    }

    return;
}

extern "C" {
#if (defined(CONFIG_CHIP_MW320_REAL_FACTORY_DATA) && (CONFIG_CHIP_MW320_REAL_FACTORY_DATA == 1))
uint8_t * __FACTORY_DATA_START;
uint32_t __FACTORY_DATA_SIZE;
#endif

void GPIO_IRQHandler(void)
{
    uint32_t intrval = GPIO_PortGetInterruptFlags(GPIO, GPIO_PORT(BOARD_SW1_GPIO_PIN));

    // Clear the interrupt
    GPIO_PortClearInterruptFlags(GPIO, GPIO_PORT(BOARD_SW1_GPIO_PIN), intrval);
    // Check which sw tiggers the interrupt
    if (intrval & 1UL << GPIO_PORT_PIN(BOARD_SW1_GPIO_PIN))
    {
        PRINTF("SW_1 click => do switch handler\r\n");
        /* Change state of button. */
        g_ButtonPress++;
        need2sync_sw_attr = true;
    }
    else if (intrval & 1UL << GPIO_PORT_PIN(BOARD_SW2_GPIO_PIN))
    {
        PRINTF("SW_2 click \r\n");
        sw2_handle(true);
    }
    SDK_ISR_EXIT_BARRIER;
}
}

int init_mw320_sdk(int (*wlan_event_callback)(enum wlan_event_reason reason, void * data))
{
    flash_desc_t fl;
    struct partition_entry *p, *f1, *f2;
    short history = 0;
    uint32_t * wififw;
    struct partition_entry * psm;
#if (defined(CONFIG_CHIP_MW320_REAL_FACTORY_DATA) && (CONFIG_CHIP_MW320_REAL_FACTORY_DATA == 1))
    struct partition_entry * manu_dat;
    uint8_t * pmfdat;
#endif // CONFIG_CHIP_MW320_REAL_FACTORY_DATA

    ChipLogProgress(NotSpecified, " init mw320 sdk ");
    if (wlan_event_callback == NULL) {
        return -WM_E_INVAL;
    }

    mcuInitPower();
    boot_init();
    get_hash_from_uninit_mem();
    mflash_drv_init();
    cli_init();
    part_init();

    psm = part_get_layout_by_id(FC_COMP_PSM, NULL);
    part_to_flash_desc(psm, &fl);
    init_flash_storage((char *) CONNECTION_INFO_FILENAME, &fl);
    ChipLogProgress(NotSpecified, "[PSM]: (start, len)=(0x%lx, 0x%lx)", fl.fl_start, fl.fl_size);

#if (defined(CONFIG_CHIP_MW320_REAL_FACTORY_DATA) && (CONFIG_CHIP_MW320_REAL_FACTORY_DATA == 1))
    manu_dat = part_get_layout_by_id(FC_COMP_USER_APP, NULL);
    part_to_flash_desc(manu_dat, &fl);
    ChipLogProgress(NotSpecified, "[Manufacture_Data]: (start, len)=(0x%lx, 0x%lx)", fl.fl_start, fl.fl_size);
    pmfdat               = (uint8_t *) mflash_drv_phys2log(fl.fl_start, fl.fl_size);
    __FACTORY_DATA_START = pmfdat;
    __FACTORY_DATA_SIZE  = (uint32_t) fl.fl_size;
#endif // CONFIG_CHIP_MW320_REAL_FACTORY_DATA

    f1 = part_get_layout_by_id(FC_COMP_WLAN_FW, &history);
    f2 = part_get_layout_by_id(FC_COMP_WLAN_FW, &history);
    if (f1 && f2)
    {
        p = part_get_active_partition(f1, f2);
    }
    else if (!f1 && f2)
    {
        p = f2;
    }
    else if (!f2 && f1)
    {
        p = f1;
    }
    else
    {
        // PRINTF("[%s]: Wi-Fi Firmware not detected\r\n", __FUNCTION__);
        p = NULL;
    }
    if (p != NULL)
    {
        part_to_flash_desc(p, &fl);
        wififw = (uint32_t *) mflash_drv_phys2log(fl.fl_start, fl.fl_size);
        // assert(wififw != NULL);
        /* First word in WIFI firmware is magic number. */
        assert(*wififw == (('W' << 0) | ('L' << 8) | ('F' << 16) | ('W' << 24)));
        wlan_init((const uint8_t *) (wififw + 2U), *(wififw + 1U));
        wlan_start(wlan_event_callback);
        // demo_init();
        os_thread_sleep(os_msec_to_ticks(5000));
    }
    ChipLogProgress(NotSpecified, " mw320 init complete!");

    return WM_SUCCESS;
}

#ifdef MW320_EVA_BOARD
#define gpio_led_cfg(base, pin, cfg)                                                                                               \
    {                                                                                                                              \
        GPIO_PinInit(base, pin, cfg);                                                                                              \
        GPIO_PortSet(base, GPIO_PORT(pin), 1u << GPIO_PORT_PIN(pin));                                                              \
    }
#else
#define gpio_led_cfg(base, pin, cfg)                                                                                               \
    {                                                                                                                              \
        GPIO_PinInit(base, pin, cfg);                                                                                              \
    }
#endif // MW320_EVA_BOARD

#define gpio_sw_cfg(base, pin, cfg, irq, trig)                                                                                     \
    {                                                                                                                              \
        GPIO_PinInit(base, pin, cfg);                                                                                              \
        GPIO_PinSetInterruptConfig(base, pin, trig);                                                                               \
        GPIO_PortEnableInterrupts(base, GPIO_PORT(pin), 1UL << GPIO_PORT_PIN(pin));                                                \
        EnableIRQ(irq);                                                                                                            \
    }

void gpio_init(void)
{
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Init output amber led gpio off */
    gpio_led_cfg(BOARD_LED_AMBER_GPIO, BOARD_LED_AMBER_GPIO_PIN, &led_config);

    /* Init output yellow led gpio off */
    gpio_led_cfg(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, &led_config);

    /* Init/config input sw_1 GPIO. */
    gpio_sw_cfg(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN, &sw_config, BOARD_SW1_IRQ, kGPIO_InterruptFallingEdge);

    /* Init/config input sw_2 GPIO. */
    gpio_sw_cfg(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config, BOARD_SW2_IRQ, kGPIO_InterruptFallingEdge);

    /* Turn LED by default */
    led_on_off(led_amber, false);
    led_on_off(led_yellow, false);
    return;
}

// Ref: BOARD_InitPins() of repo/boards/rdmw320_r0/wifi_examples/mw_wifi_cli/pin_mux.c
void Mw320_BOARD_InitPins(void)
{                                /*!< Function assigned for the core: Cortex-M4[cm4] */
    PINMUX_PinMuxSet(BOARD_UART0_TX_PIN, BOARD_UART0_TX_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);
    PINMUX_PinMuxSet(BOARD_UART0_RX_PIN, BOARD_UART0_RX_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);
    PINMUX_PinMuxSet(BOARD_PUSH_SW1_PIN, BOARD_PUSH_SW1_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);

    PINMUX_PinMuxSet(BOARD_PUSH_SW2_PIN, BOARD_PUSH_SW2_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);
    PINMUX_PinMuxSet(BOARD_PUSH_SW4_PIN, BOARD_PUSH_SW4_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);

    PINMUX_PinMuxSet(BOARD_UART2_TX_PIN, BOARD_UART2_TX_PIN_FUNCTION_ID | BOARD_UART2_TX_PULL_STATE);
    PINMUX_PinMuxSet(BOARD_UART2_RX_PIN, BOARD_UART2_RX_PIN_FUNCTION_ID | BOARD_UART2_RX_PULL_STATE);

    PINMUX_PinMuxSet(BOARD_REQ_PIN,   BOARD_COEX_PIN_FUNCTION_ID | PINMUX_MODE_PULLDOWN);
    // Note: BOARD_PRI_PIN has a conflict with BOARD_LED_YELLOW_PIN that LED may fail to work
    //PINMUX_PinMuxSet(BOARD_PRI_PIN,   BOARD_COEX_PIN_FUNCTION_ID | PINMUX_MODE_PULLDOWN);
    ////PINMUX_PinMuxSet(BOARD_GRANT_PIN, BOARD_COEX_PIN_FUNCTION_ID | PINMUX_MODE_PULLDOWN);
    PINMUX_PinMuxSet(BOARD_LED_YELLOW_PIN, BOARD_LED_YELLOW_PIN_FUNCTION_ID | PINMUX_MODE_DEFAULT);
}

void board_init(void)
{
    /* Initialize platform */
    // BOARD_ConfigMPU();
    Mw320_BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    PRINTF("<< MW320 MCU firmware: [%s] >>\r\n", img_name_str);
#ifdef CONFIGURE_UAP
    PRINTF("\nDo you want to use the default SSID and key for mw320 uAP? [y/n]\r\n");
    do
    {
        ch = GETCHAR();
        PUTCHAR(ch);
        if (ch == 'n')
        {
            PRINTF("\nPlease input your SSID: [ 1 ~ 32 characters]\r\n");
            bp = 0;
            do
            {
                ssid[bp] = GETCHAR();
                PUTCHAR(ssid[bp]);
                bp++;
                if (bp > sizeof(ssid))
                {
                    PRINTF("\n ERROR: your SSID length=%d is larger than %d \r\n", bp, sizeof(ssid));
                    return 0;
                }
            } while (ssid[bp - 1] != '\r');
            ssid[bp - 1] = '\0';
            PRINTF("\nPlease input your KEY: [ 8 ~ 63 characters]\r\n");
            bp = 0;
            do
            {
                psk[bp] = GETCHAR();
                PUTCHAR(psk[bp]);
                bp++;
                if (bp > sizeof(psk))
                {
                    PRINTF("\n ERROR: your KEY length=%d is larger than %d \r\n", bp, sizeof(psk));
                    return 0;
                }
            } while (psk[bp - 1] != '\r');
            psk[bp - 1] = '\0';
            if ((bp - 1) < 8)
            {
                PRINTF("\n ERROR: KEY length=%d is less than 8 \r\n", (bp - 1));
                return 0;
            }
            break;
        }
        if (ch == '\r')
        {
            break;
        }
    } while (ch != 'y');
#endif
    //    PRINTF("\nMW320 uAP SSID=%s key=%s ip=%s \r\n", ssid, psk, network_ip);

#ifdef MW320_EVA_BOARD
    CLOCK_EnableXtal32K(kCLOCK_Osc32k_External);
    CLOCK_AttachClk(kXTAL32K_to_RTC);
#endif // MW320_EVA_BOARD
    aesLock = xSemaphoreCreateRecursiveMutex();
    assert(aesLock != NULL);

    AES_Init(APP_AES);
    AES_SetLockFunc(APP_AES_Lock, APP_AES_Unlock);
    gpio_init();

	return;
}

AppTask AppTask::sAppTask;
op_state_t AppTask::ReadOpState(void)
{
    op_state_t op_state = frst_state;
    int ret;
    unsigned char ops_buf[1];
    uint32_t len=1;

    ret = get_saved_wifi_network((char *) DEV_OP_STATE_FNAME, ops_buf, &len);
    if (ret != WM_SUCCESS) {
        // Can't get the device commission state => It's right after doing factory_reset
        return frst_state;
    }
    op_state = (op_state_t)ops_buf[0];
    return op_state;
}

void AppTask::WriteOpState(op_state_t opstat)
{
    int ret;

    ret = save_wifi_network((char *)DEV_OP_STATE_FNAME, (uint8_t *)&opstat, sizeof(uint8_t));
    if (ret != WM_SUCCESS)
    {
        PRINTF("Error: save comm_state to flash failed\r\n");
    }

    return;
}

void AppTask::SetOpState(op_state_t opstat)
{
    OpState = opstat;
    WriteOpState(opstat);
    return;
};

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // Get the device commission state
#if (MW320_FEATURE_UAP == 0)
    // If uap is not enabled => switching to work_state directly
    SetOpState(work_state);
#else
    OpState = ReadOpState();
#endif
    switch (OpState) {
        case frst_state:
            // start uap at initialization if it's not commissioned or uap_commissioned
            UapInit();
            UapOnoff(true);
            break;
        case work_state:
        default:
            char ssid[IEEEtypes_SSID_SIZE + 1];
            char psk[WLAN_PSK_MAX_LENGTH];
            LoadNetwork(ssid, psk);
            if ((strlen(ssid) == 0) || (strlen(psk) == 0)) {
                // Has moved to the work_state, but AP information is missing or insufficient => Use the default ssid/password
                SaveNetwork((char*)DEFAP_SSID, (char*)DEFAP_PWD);
                strcpy(ssid, DEFAP_SSID);
                strcpy(psk, DEFAP_PWD);
            }
            ChipLogProgress(NotSpecified, "Connecting to [%s, %s]", ssid, psk);
            chip::DeviceLayer::Internal::ConnectivityUtils::ConnectWiFiNetwork(ssid, psk);
            break;
    }
    return err;
}


void AppTask::AppTaskMain(void *pvParameter)
{
    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        PRINTF("AppTask.Init() failed\r\n");
        return;
    }
    while (true)
    {
        /* wait for interface up */
        os_thread_sleep(os_msec_to_ticks(500));
        /*PRINTF("[%s]: looping\r\n", __FUNCTION__);*/
        if (need2sync_sw_attr == true)
        {
            static bool is_on = false;
            uint16_t value    = g_ButtonPress & 0x1;
            is_on             = !is_on;
            value             = (uint16_t) is_on;
            // sync-up the switch attribute:
            ChipLogProgress(NotSpecified, "--> update Switch::Attributes::CurrentPosition::Id [%d]", value);
            emAfWriteAttribute(1, Switch::Id, Switch::Attributes::CurrentPosition::Id, (uint8_t *) &value, sizeof(value), true,
                               false);
#ifdef SUPPORT_MANUAL_CTRL
            // sync-up the Light attribute (for test event, OO.M.ManuallyControlled)
            ChipLogProgress(NotSpecified, "--> update [OnOff::Id]: OnOff::Attributes::OnOff::Id [%d]", value);
            emAfWriteAttribute(1, OnOff::Id, OnOff::Attributes::OnOff::Id, (uint8_t *) &value, sizeof(value), true, false);
#endif // SUPPORT_MANUAL_CTRL
#if (MW320_FEATURE_BIND == 1)
	    ChipLogProgress(NotSpecified, "Bind notify status: (%u, %u)", BindingTable::GetInstance().Size(), bnd_notify_done);
            // Trigger to send on/off/toggle command to the bound devices
            if ((BindingTable::GetInstance().Size()>0) && (bnd_notify_done == true)) {
                bnd_notify_done = false;
                TriggerBinding_all();
            } else {
                ChipLogProgress(NotSpecified, "Last command is executing or haven't bound yet, skip the new ones");
            }
#endif //MW320_FEATURE_BIND
            need2sync_sw_attr = false;
        }
        // =============================
        // Call sw2_handle to clear click_count if needed
        sw2_handle(false);
    }

    return;
}

// uAP functions
#define WIFI_AP_SSID	"mw320_uap"
#define WIFI_AP_IP_ADDR  "192.168.1.1"
#define WIFI_AP_NET_MASK "255.255.0.0" /* IP address configuration. */
#define WIFI_PASSWORD "nxp12345"
#define WIFI_AP_CHANNEL 1
static const char def_uap_ssid[]={WIFI_AP_SSID};
static const char def_uap_ip[]={WIFI_AP_IP_ADDR};
static const char def_uap_pwd[]={WIFI_PASSWORD};

// Initialize uap
int AppTask::UapInit(void)
{
    int ret;
    unsigned int uap_ip = ipaddr_addr(def_uap_ip);
    uint8_t ssid_len = 0;
    uint8_t psk_len  = 0;
    wlan_initialize_uap_network(&uap_network);
    /* Set IP address to WIFI_AP_IP_ADDR */
    uap_network.ip.ipv4.address = uap_ip;
    /* Set default gateway to WIFI_AP_IP_ADDR */
    uap_network.ip.ipv4.gw = uap_ip;
    /* Set SSID as passed by the user */
    ssid_len = (strlen(def_uap_ssid) <= IEEEtypes_SSID_SIZE) ? strlen(def_uap_ssid) : IEEEtypes_SSID_SIZE;
    memcpy(uap_network.ssid, def_uap_ssid, ssid_len);
    uap_network.channel = WIFI_AP_CHANNEL;
    uap_network.security.type = WLAN_SECURITY_WPA2;
    /* Set the passphrase. Max WPA2 passphrase can be upto 64 ASCII chars */
    psk_len = (strlen(def_uap_pwd) <= (WLAN_PSK_MAX_LENGTH - 1)) ? strlen(def_uap_pwd) : (WLAN_PSK_MAX_LENGTH - 1);
    strncpy(uap_network.security.psk, def_uap_pwd, psk_len);
    uap_network.security.psk_len = psk_len;
    ret = wlan_add_network(&uap_network);
    if (ret != WM_SUCCESS) {
        PRINTF("Initialize uAP failed \r\n");
        return WM_FAIL;
    }
    ChipLogProgress(NotSpecified, "Intialize uAP successfully \r\n");
    return WM_SUCCESS;
}

/*
    Turn on/off uAP
*/
void AppTask::UapOnoff(bool do_on)
{
    int ret;
    if (do_on == true) {    // Turn on uAP
        ChipLogProgress(NotSpecified, "Turning on uAP");
        ret = wlan_start_network(uap_network.name);
        if (ret != WM_SUCCESS) {
            PRINTF("Failed to turn on uAP (%d)\r\n", ret);
        } else {
            ChipLogProgress(NotSpecified, "start uAP sucessfully");
        }
    } else {    // Turn off uAP
        PRINTF("Turning off uAP \r\n");
        if (is_uap_started()) {
            wlan_get_current_uap_network(&uap_network);
            ret = wlan_stop_network(uap_network.name);
            if (ret != WM_SUCCESS)
                PRINTF("Error: unable to stop network\r\n");
            else
                ChipLogProgress(NotSpecified, "stop uAP, SSID = [%s]", uap_network.ssid);
        }
    }
    return;
}

void AppTask::LoadNetwork(char * ssid, char * pwd)
{
    int ret;
    unsigned char ssid_buf[IEEEtypes_SSID_SIZE + 1];
    unsigned char psk_buf[WLAN_PSK_MAX_LENGTH];
    uint32_t len;

    len = IEEEtypes_SSID_SIZE + 1;
    ret = get_saved_wifi_network((char *) SSID_FNAME, ssid_buf, &len);
    if (ret != WM_SUCCESS)
    {
        PRINTF("Error: Read saved SSID\r\n");
        strcpy(ssid, "");
    }
    else
    {
        ChipLogProgress(NotSpecified, "saved_ssid: [%s]", ssid_buf);
        strcpy(ssid, (const char *) ssid_buf);
    }

    len = WLAN_PSK_MAX_LENGTH;
    ret = get_saved_wifi_network((char *) PSK_FNAME, psk_buf, &len);
    if (ret != WM_SUCCESS)
    {
        PRINTF("Error: Read saved PSK\r\n");
        strcpy(pwd, "");
    }
    else
    {
        ChipLogProgress(NotSpecified, "saved_psk: [%s]\r\n", psk_buf);
        strcpy(pwd, (const char *) psk_buf);
    }
}

void AppTask::SaveNetwork(char * ssid, char * pwd)
{
    int ret;

    ret = save_wifi_network((char *) SSID_FNAME, (uint8_t *) ssid, strlen(ssid) + 1);
    if (ret != WM_SUCCESS)
    {
        PRINTF("Error: write ssid to flash failed\r\n");
    }

    ret = save_wifi_network((char *) PSK_FNAME, (uint8_t *) pwd, strlen(pwd) + 1);
    if (ret != WM_SUCCESS)
    {
        PRINTF("Error: write psk to flash failed\r\n");
    }

    return;
}

#if (MW320_FEATURE_BIND == 1)
void AppTask::TriggerBinding_all(void)
{
    for (auto iter = BindingTable::GetInstance().begin(); iter != BindingTable::GetInstance().end(); ++iter)
    {
        if (iter->type == EMBER_UNICAST_BINDING)
        {
            chip::BindingManager::GetInstance().NotifyBoundClusterChanged(iter->local, iter->clusterId.Value(), nullptr);
        }
    }

    return;
}
#endif //MW320_FEATURE_BIND
