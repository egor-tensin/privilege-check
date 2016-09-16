#include "error.hpp"
#include "os.hpp"
#include "process.hpp"
#include "resource.hpp"
#include "resource_ids.h"
#include "sid.hpp"
#include "token.hpp"

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

void set_label(HWND root, int id, const wchar_t* s)
{
    const auto label = GetDlgItem(root, id);
    SetWindowTextW(label, s);
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
        set_label(wnd, IDC_ADMINISTRATOR, is_user_in_administrators());
    }
    catch (const Error& e)
    {
        set_label(wnd, IDC_ADMINISTRATOR, L"N/A");
        error::report(e);
    }

    try
    {
        set_label(wnd, IDC_RUN_AS_ADMINISTRATOR, is_run_as_administrator());
    }
    catch (const Error& e)
    {
        set_label(wnd, IDC_RUN_AS_ADMINISTRATOR, L"N/A");
        error::report(e);
    }

    if (os::is_vista_or_later())
    {
        try
        {
            const auto elevated = is_elevated();
            set_label(wnd, IDC_ELEVATED, elevated);

            const auto elevate_button = GetDlgItem(wnd, IDC_BUTTON_ELEVATE);
            Button_SetElevationRequiredState(elevate_button, !elevated);
        }
        catch (const Error& e)
        {
            set_label(wnd, IDC_ELEVATED, L"N/A");
            error::report(e);
        }

        try
        {
            set_label(wnd, IDC_INTEGRITY_LEVEL, token::integrity_level_to_string(
                token::query_integrity_level(token::open_for_current_process())));
        }
        catch (const Error& e)
        {
            set_label(wnd, IDC_INTEGRITY_LEVEL, L"N/A");
            error::report(e);
        }
    }
    else
    {
        set_label(wnd, IDC_ELEVATED, L"N/A");
        set_label(wnd, IDC_INTEGRITY_LEVEL, L"N/A");
    }

    return TRUE;
}

void report_already_elevated(HWND wnd)
{
    MessageBoxW(wnd,
        resource::load_string(IDS_ALREADY_ELEVATED).c_str(),
        resource::load_string(IDS_ELEVATION_CAPTION).c_str(),
        MB_OK);
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
        report_already_elevated(wnd);
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
            error::report(error::make("DialogBoxW"));
        default:
            return static_cast<int>(ret);
    }
}
