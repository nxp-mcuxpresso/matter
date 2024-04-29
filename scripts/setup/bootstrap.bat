:<<"::WINDOWS_ONLY"
@echo off
:: Copyright 2023 Project CHIP Authors
::
:: Licensed under the Apache License, Version 2.0 (the "License"); you may not
:: use this file except in compliance with the License. You may obtain a copy of
:: the License at
::
::     https://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
:: WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
:: License for the specific language governing permissions and limitations under
:: the License.
::
:<<"::WINDOWS_ONLY"

:: The following Python checks are extracted from the official bootstrap
:: script found in the Pigweed repository.

:: Allow forcing a specific Python version through the environment variable
:: PW_BOOTSTRAP_PYTHON. Otherwise, use the system Python if one exists.
if not "%PW_BOOTSTRAP_PYTHON%" == "" (
	set "python=%PW_BOOTSTRAP_PYTHON%"
	goto check_curl
)

setlocal EnableDelayedExpansion
:: Detect python installation.
for /f %%p in ('where python') do (
	%%p --version >NUL 2>&1
	if !ERRORLEVEL! EQU 0 (
		endlocal
		echo Python is found at: %%p
		set "python=%%p"
		goto check_curl
	)
)

echo.
echo Error: no system Python present
echo.
echo   Pigweed's bootstrap process requires a local system Python.
echo   Please install Python on your system, add it to your PATH
echo   and re-try running bootstrap.
goto finish

:: Detect curl installation
:check_curl
where curl >NUL 2>&1
if %ERRORLEVEL% NEQ 0 (
	echo.
	echo Error: no system curl present
	echo.
	echo   Pigweed's prerequisites check requires a local system curl.
	echo   Please install curl on your system, add it to your PATH
	echo   and re-try running bootstrap.
	goto finish
)

set _BOOTSTRAP_NAME=%~n0
set _CHIP_ROOT=%cd%
set _CONFIG_FILE=%_CHIP_ROOT%\scripts\setup\environment.json

set PW_BRANDING_BANNER=%_CHIP_ROOT%\scripts\setup\banner.txt
set PW_DOCTOR_SKIP_CIPD_CHECKS=1
set PW_PROJECT_ROOT=%_CHIP_ROOT%
set PW_ENVIRONMENT_ROOT=%PW_PROJECT_ROOT%\.environment
set PW_ROOT=%PW_PROJECT_ROOT%\third_party\pigweed\repo
:: Skip pw environment check for now. To be removed in the future.
set PW_ACTIVATE_SKIP_CHECKS=1

set _PIGWEED_CIPD_JSON=%PW_PROJECT_ROOT%/third_party/pigweed/repo/pw_env_setup/py/pw_env_setup/cipd_setup/pigweed.json
set _PW_ACTUAL_ENVIRONMENT_ROOT=%PW_ENVIRONMENT_ROOT%
set _SETUP_BAT=%_PW_ACTUAL_ENVIRONMENT_ROOT%\activate.bat

if "%_BOOTSTRAP_NAME%" == "bootstrap" (
	git submodule sync --recursive
	git submodule update
)

if not exist "%_PW_ACTUAL_ENVIRONMENT_ROOT%" mkdir "%_PW_ACTUAL_ENVIRONMENT_ROOT%"
set _GENERATED_PIGWEED_CIPD_JSON=%_PW_ACTUAL_ENVIRONMENT_ROOT%/pigweed.json

call "%python%" "scripts/setup/gen_pigweed_cipd_json.py" -i "%_PIGWEED_CIPD_JSON%" -o "%_GENERATED_PIGWEED_CIPD_JSON%"
if %ERRORLEVEL% NEQ 0 goto finish

if "%_BOOTSTRAP_NAME%" == "bootstrap" (
	goto pw_check_prerequisites
) else (
	call "%python%" "%PW_ROOT%\pw_env_setup\py\pw_env_setup\windows_env_start.py"
	if %ERRORLEVEL% NEQ 0 goto finish
	goto pw_activate
)

:: Ensure pigweed env prerequisites are OK
:pw_check_prerequisites
curl https://pigweed.googlesource.com/pigweed/examples/+/main/tools/setup_windows_prerequisites.bat?format=TEXT > setup_pigweed_prerequisites.b64
certutil -decode -f setup_pigweed_prerequisites.b64 setup_pigweed_prerequisites.bat
del setup_pigweed_prerequisites.b64
call setup_pigweed_prerequisites.bat
if %ERRORLEVEL% NEQ 0 goto finish
goto pw_bootstrap

:install_additional_pip_requirements
pip install ^
	-q ^
	-r "%_CHIP_ROOT%\scripts\setup\requirements.all.txt" ^
	-c "%_CHIP_ROOT%\scripts\setup\constraints.txt"
goto pw_cleanup

:pw_bootstrap
call "%python%" "%PW_ROOT%\pw_env_setup\py\pw_env_setup\env_setup.py" ^
	--pw-root "%PW_ROOT%" ^
	--shell-file "%_SETUP_BAT%" ^
	--install-dir "%_PW_ACTUAL_ENVIRONMENT_ROOT%" ^
	--config-file "%_CONFIG_FILE%" ^
	--virtualenv-gn-out-dir "%_PW_ACTUAL_ENVIRONMENT_ROOT%\gn_out" ^
	--project-root "%PW_PROJECT_ROOT%" ^
    --additional-cipd-file "%_GENERATED_PIGWEED_CIPD_JSON%"
if %ERRORLEVEL% NEQ 0 goto finish
goto pw_activate

:pw_activate
call "%_SETUP_BAT%"
if "%_BOOTSTRAP_NAME%" == "bootstrap" (
	goto install_additional_pip_requirements
) else (
	goto pw_cleanup
)

:pw_cleanup
set _PW_BANNER=
set _PW_BANNER_FUNC=
set PW_BANNER_FUNC=
set _PW_ENV_SETUP=
set _PW_NAME=
set PW_ENVIRONMENT_ROOT=
set _PW_PYTHON=
set _PW_ENV_ROOT_TXT=
set _PW_PREV_ENV_ROOT=
set _PW_SETUP_SH=
set _PW_DEACTIVATE_SH=
set _NEW_PW_ROOT=
set _PW_ENV_SETUP_STATUS=
set _PW_ENV_PREFIX=
set _PW_ENV=
set _PW_DOTENV=
goto finish

:finish
set _CHIP_ROOT=
set _BOOTSTRAP_NAME=
set PW_CIPD_INSTALL_DIR=
set _PW_TEXT=
set PW_DOCTOR_SKIP_CIPD_CHECKS=
if exist setup_pigweed_prerequisites.bat del setup_pigweed_prerequisites.bat
exit /B 0
