/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Application version */
#define APP_MAJ_VER                    0x01
#define APP_MIN_VER                    0x00
#define APP_BLD_VER                    0x0000

/* Define this to 1 when building for reference design (16 MB flash)
 * Otherwise the project assumes flash size is 32 MB */
#define SLN_SVUI_RD                    0

/* Choose the ASR engine: DSMT, VIT or S2I
*  The default lib used is the one for S2I. When switching to VIT,
*  it is required to comment line 445 and uncomment line 446 in the
*  following file: third_party/nxp/rt_sdk/rt_sdk.gni */
#define ENABLE_DSMT_ASR                0
#define ENABLE_VIT_ASR                 0
#define ENABLE_S2I_ASR                 1

#if ENABLE_DSMT_ASR
/* Set USE_DSMT_EVALUATION_MODE to 1 when using DSMT evaluation library.
 * DSMT evaluation library is limited to 100 detections. When this limit is reached
 * the commands detection will stop. A power reset will be needed afterwards.
 * DSMT production library MUST be used when going to production. */
#define USE_DSMT_EVALUATION_MODE       1

/* Pool allocation method. When set to 1, static allocation will be used.
 * When set to 0, dynamic allocation from FreeRTOS heap will be used.  */
#define USE_DSMT_STATIC_POOLS          1
#endif /* ENABLE_DSMT_ASR */

/* Enable Voice Activity Detection */
#define ENABLE_VAD                     0

#if ENABLE_VAD
/* The period of silence after which low power is enabled
 * During the low power the MCU frequency is decreased and
 * ASR is bypassed */
#define VAD_LOW_POWER_AFTER_SEC        10

/* If set to 1, it will activate the audio buffering during VAD low power mode.
 * After waking up from VAD low power, the ASR will first process the buffered
 * audio, before catching up with real time audio
 *
 * This setting will use 10KB more FreeRTOS heap */
#define VAD_BUFFER_DATA                1
#endif /* ENABLE_VAD */

/* Choose microphones type. If using PDM configuration,
 * switch the definition to MICS_PDM */
#define MICS_TYPE                      MICS_I2S

/* Speaker volume, between 0 and 100 */
#define DEFAULT_SPEAKER_VOLUME         55

/* Enable usb audio dump by setting this define on 1 */
#define ENABLE_USB_AUDIO_DUMP          0

/* Max wake word length. Might consider switching to 1500
 * for shorter wake words. We are using 3seconds to accommodate
 * larger utterances like "Salut NXP" */
#define WAKE_WORD_MAX_LENGTH_MS        3000

/* Enable Acoustic Echo Cancellation. When set to 0, barge-in will not work.
 * Disabling saves RAM memory. */
#define ENABLE_AEC                     0

/* Enable NXP out of the box experience. If set to 0,
 * no demo change or language change available through voice commands,
 * but these actions will still be possible through shell commands. */
#define ENABLE_NXP_OOBE                1

/* Enable amplifier, i.e. audio prompts played on the speaker.
 * Currently the implementation includes playing OPUS encoded
 * prompts from the file system */
#define ENABLE_AMPLIFIER               0

#if ENABLE_AMPLIFIER
/* If set to 1, streamer task will run and it will decode OPUS encoded
 * audio files, then feed raw PCM audio data to the amplifier.
 * If set to 0, streamer task will not feed raw PCM audio data to the amplifier.
 * Disabling the streamer saves RAM memory. */
#define ENABLE_STREAMER                1
#endif /* ENABLE_AMPLIFIER */

/* Enable CPU usage tracing. When set to 1, a new command will be available in
 * sln_shell: cpuview. This will print CPU usage per task */
#define SLN_TRACE_CPU_USAGE            0

/* Enable logging task based on dynamic buffer allocation.
 * Using this is helpful as the other tasks do not need to waste time printing
 * on the console. The log is instead inserted in a queue and printed by the
 * logging task, which will minimize the impact of logging for all tasks.
 *
 * This setting will use 1.5KB more FreeRTOS heap */
#define ENABLE_LOGGING_TASK            0

/* Choose logging interface: either through shell (USB or UART) or UART debug console.
 * The shell implementation offers a set of pre-implemented useful commands. Use command
 * "help" in shell for more details.
 *
 * UART debug console will use the least RAM.
 * UART shell will use 2.7KB more static RAM and 2.2KB more FreeRTOS Heap than UART debug console
 * USB shell will use 8.5KB more static RAM than UART shell */
#define ENABLE_SHELL                   0
#define ENABLE_UART_CONSOLE            1

#if ENABLE_SHELL
/* Choose either USB shell or UART shell */
#define ENABLE_USB_SHELL               1
#define ENABLE_UART_SHELL              0

#define SHELL_BUFFER_SIZE              100
#endif /* ENABLE_SHELL */

#include "app_nonconfig_settings.h"
