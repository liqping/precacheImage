#include "flutter_window.h"

#include <optional>
#include "flutter/generated_plugin_registrant.h"
#include <dwmapi.h>


LRESULT CALLBACK child_window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR subclassID, DWORD_PTR refData)
{
    HDC hdc;
    switch (message){
        case WM_PAINT:
            PAINTSTRUCT ps;
             hdc = BeginPaint(window, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(GetStockObject(WHITE_BRUSH)));

            EndPaint(window, &ps);
            return 0;

        case WM_ERASEBKGND:
            hdc = (HDC)wparam;
            RECT rec;
            GetClientRect(window, &rec);
      
            FillRect(hdc, &rec, (HBRUSH)(GetStockObject(WHITE_BRUSH)));

            return 1L;
    }
    return DefSubclassProc(window, message, wparam, lparam);
}

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
    : project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
  if (!Win32Window::OnCreate()) {
    return false;
  }

  RECT frame = GetClientArea();

  // The size here must match the window dimensions to avoid unnecessary surface
  // creation / destruction in the startup path.
  flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
      frame.right - frame.left, frame.bottom - frame.top, project_);
  // Ensure that basic setup of the controller was successful.
  if (!flutter_controller_->engine() || !flutter_controller_->view()) {
    return false;
  }
  RegisterPlugins(flutter_controller_->engine());
  auto hwnd = flutter_controller_->view()->GetNativeWindow();
  //::SetWindowSubclass(hwnd, child_window_proc, 1, NULL);
  SetChildContent(hwnd);
 
  flutter_controller_->engine()->SetNextFrameCallback([&]() {
    this->Show();
   
  });



  return true;
}

void FlutterWindow::OnDestroy() {
  if (flutter_controller_) {
    flutter_controller_ = nullptr;
  }

  Win32Window::OnDestroy();
}

LRESULT
FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
                              WPARAM const wparam,
                              LPARAM const lparam) noexcept {
  // Give Flutter, including plugins, an opportunity to handle window messages.
  if (flutter_controller_) {
    std::optional<LRESULT> result =
        flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
                                                      lparam);
    if (result) {
      return *result;
    }
  }

  switch (message) {
    case WM_FONTCHANGE:
      flutter_controller_->engine()->ReloadSystemFonts();
      break;
  }

  return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
