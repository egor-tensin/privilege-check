#include "error.h"
#include "os.h"
#include "process.h"
#include "resource_ids.h"
#include "sid.h"
#include "token.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <windowsx.h>

#include <string>

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

bool is_user_in_administrators()
{
    const auto token = token::impersonate(
        token::get_linked(
            token::open_for_current_process(token::permissions::query() | token::permissions::duplicate())));

    return token::belongs(token, sid::builtin_administrators());
}

bool is_run_as_administrator()
{
    return token::belongs(token::dumb(), sid::builtin_administrators());
}

bool is_elevated()
{
    return token::is_elevated(token::open_for_current_process());
}

void set_label(HWND root, int id, bool val)
{
    const auto label = GetDlgItem(root, id);
    SetWindowTextW(label, val ? L"True" : L"False");
}

void set_label(HWND root, int id, const std::wstring& s)
{
    const auto label = GetDlgItem(root, id);
    SetWindowTextW(label, s.c_str());
}

BOOL on_init_dialog(HWND wnd, HWND, LPARAM)
{
    try
    {
        set_label(wnd, IDC_STATIC_IS_ADMIN, is_user_in_administrators());
    }
    catch (const Error& e)
    {
        set_label(wnd, IDC_STATIC_IS_ADMIN, L"N/A");
        error::report(e);
    }

    try
    {
        set_label(wnd, IDC_STATIC_IS_RUN_AS_ADMIN, is_run_as_administrator());
    }
    catch (const Error& e)
    {
        set_label(wnd, IDC_STATIC_IS_RUN_AS_ADMIN, L"N/A");
        error::report(e);
    }

    if (os::is_vista_or_later())
    {
        try
        {
            const auto elevated = is_elevated();
            set_label(wnd, IDC_STATIC_IS_ELEVATED, elevated);

            const auto elevate_button = GetDlgItem(wnd, IDC_BUTTON_ELEVATE);
            Button_SetElevationRequiredState(elevate_button, !elevated);
        }
        catch (const Error& e)
        {
            set_label(wnd, IDC_STATIC_IS_ELEVATED, L"N/A");
            error::report(e);
        }

        try
        {
            set_label(wnd, IDC_STATIC_INTEGRITY_LEVEL, token::integrity_level_to_string(
                token::query_integrity_level(token::open_for_current_process())));
        }
        catch (const Error& e)
        {
            set_label(wnd, IDC_STATIC_INTEGRITY_LEVEL, L"N/A");
            error::report(e);
        }
    }
    else
    {
        set_label(wnd, IDC_STATIC_IS_ELEVATED, L"N/A");
        set_label(wnd, IDC_STATIC_INTEGRITY_LEVEL, L"N/A");
    }

    return TRUE;
}

void on_button_elevate_click(HWND wnd)
{
    bool as_admin = false;

    try
    {
        as_admin = is_run_as_administrator();
    }
    catch (const Error& e)
    {
        error::report(e);
        return;
    }

    if (as_admin)
    {
        MessageBoxW(wnd, L"Already elevated!", L"Elevation", MB_OK);
        return;
    }

    try
    {
        process::runas(process::get_executable_path());
    }
    catch (const Error& e)
    {
        if (error::get_code(e) != ERROR_CANCELLED)
            error::report(e);
        return;
    }

    EndDialog(wnd, 1);
}

void on_command(HWND wnd, int id, HWND, unsigned int)
{
    switch (id)
    {
        case IDC_BUTTON_ELEVATE:
            on_button_elevate_click(wnd);
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(wnd, 0);
            break;
    }
}

void on_close(HWND wnd)
{
    EndDialog(wnd, 0);
}

INT_PTR CALLBACK dialog_main(
    HWND wnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(wnd, WM_INITDIALOG, on_init_dialog);
        HANDLE_MSG(wnd, WM_COMMAND, on_command);
        HANDLE_MSG(wnd, WM_CLOSE, on_close);

        default:
            return FALSE;
    }
}

int APIENTRY wWinMain(
    HINSTANCE instance,
    HINSTANCE,
    wchar_t*,
    int)
{
    const auto ret = DialogBoxW(
        instance,
        MAKEINTRESOURCE(IDD_MAINDIALOG),
        NULL,
        dialog_main);

    switch (ret)
    {
        case -1:
            try
            {
                error::raise("DialogBoxW");
            }
            catch (const Error& e)
            {
                error::report(e);
                return 1;
            }
            break;

        default:
            return static_cast<int>(ret);
    }
}
