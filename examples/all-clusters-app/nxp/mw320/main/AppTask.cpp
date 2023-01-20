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

/* platform specific */
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

#include <wm_os.h>
extern "C" {
#include "boot_flags.h"
#include "cli.h"
#include "dhcp-server.h"
#include "iperf.h"
#include "mflash_drv.h"
#include "network_flash_storage.h"
#include "partition.h"
#include "ping.h"
#include "wlan.h"
#include "wm_net.h"
}
#include "fsl_aes.h"
#include "lpm.h"

#define RUN_RST_LT_DELAY 10
#define APP_AES AES
#define CONNECTION_INFO_FILENAME "connection_info.dat"

volatile int g_ButtonPress = 0;
bool need2sync_sw_attr     = false;

//using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace chip;

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


AppTask AppTask::sAppTask;


CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    return err;
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
        PRINTF(" (%d times) \r\n", click_cnt);
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

    PRINTF("=> init mw320 sdk \r\n");
	if (wlan_event_callback == NULL) {
		return -WM_E_INVAL;
	}

    PRINTF("call mcuInitPower() \r\n");
    mcuInitPower();
    boot_init();
    get_hash_from_uninit_mem();
    mflash_drv_init();
    cli_init();
    part_init();

    psm = part_get_layout_by_id(FC_COMP_PSM, NULL);
    part_to_flash_desc(psm, &fl);
    init_flash_storage((char *) CONNECTION_INFO_FILENAME, &fl);
    PRINTF("[PSM]: (start, len)=(0x%x, 0x%x)\r\n", fl.fl_start, fl.fl_size);

#if (defined(CONFIG_CHIP_MW320_REAL_FACTORY_DATA) && (CONFIG_CHIP_MW320_REAL_FACTORY_DATA == 1))
    manu_dat = part_get_layout_by_id(FC_COMP_USER_APP, NULL);
    part_to_flash_desc(manu_dat, &fl);
    PRINTF("[Manufacture_Data]: (start, len)=(0x%x, 0x%x)\r\n", fl.fl_start, fl.fl_size);
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
        // PRINTF("[%s]: wlan_init success \r\n", __FUNCTION__);
        wlan_start(wlan_event_callback);
        // demo_init();
        os_thread_sleep(os_msec_to_ticks(5000));
    }
    PRINTF(" mw320 init complete! \r\n");

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

void board_init(void)
{
    /* Initialize platform */
    // BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    PRINTF("=====> MW320 MCU firmware \r\n");
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

