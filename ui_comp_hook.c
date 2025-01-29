#include <lvgl.h>
#include <TFT_eSPI.h>  // Thư viện hỗ trợ TFT màn hình (nếu dùng TFT ILI9341)

TFT_eSPI tft = TFT_eSPI();  // Khởi tạo đối tượng TFT

// Định nghĩa các biến cho Button và Text Area
lv_obj_t *btn1;
lv_obj_t *txt_area;
lv_obj_t *keyboard;
lv_obj_t *password_txt_area;
lv_obj_t *password_label;
lv_obj_t *btn_confirm;

// Mật khẩu hợp lệ
const char* valid_password = "1234";  // Thay mật khẩu này bằng mật khẩu bạn muốn kiểm tra

// Hàm khởi tạo màn hình và GUI
void setup() {
  // Khởi tạo màn hình TFT
  tft.begin();
  tft.setRotation(3);  // Điều chỉnh xoay màn hình nếu cần
  lv_init();  // Khởi tạo LVGL

  // Khởi tạo hiển thị cho LVGL
  lv_disp_buf_t disp_buf;
  static lv_color_t buf[LV_HOR_RES_MAX * 10];
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  // Khởi tạo màn hình 1 (Screen 1)
  lv_obj_t *screen1 = lv_obj_create(NULL, NULL);
  lv_scr_load(screen1);  // Chuyển màn hình hiện tại sang screen1

  // Tạo Button trên màn hình 1
  btn1 = lv_btn_create(screen1, NULL);
  lv_obj_set_size(btn1, 100, 50);
  lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_event_cb(btn1, btn1_event_cb);

  // Tạo Label cho Button
  lv_obj_t *label = lv_label_create(btn1, NULL);
  lv_label_set_text(label, "Show Keyboard");

  // Tạo Text Area cho nhập mật khẩu (ẩn ban đầu)
  password_txt_area = lv_textarea_create(screen1, NULL);
  lv_obj_set_size(password_txt_area, 200, 40);
  lv_obj_align(password_txt_area, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 40);
  lv_textarea_set_password_mode(password_txt_area, true); // Chế độ mật khẩu
  lv_obj_add_flag(password_txt_area, LV_OBJ_FLAG_HIDDEN);  // Ẩn Text Area

  // Tạo nút xác nhận mật khẩu
  btn_confirm = lv_btn_create(screen1, NULL);
  lv_obj_set_size(btn_confirm, 100, 50);
  lv_obj_align(btn_confirm, NULL, LV_ALIGN_IN_TOP_LEFT, 220, 40);
  lv_obj_set_event_cb(btn_confirm, btn_confirm_event_cb);
  lv_obj_t *btn_confirm_label = lv_label_create(btn_confirm, NULL);
  lv_label_set_text(btn_confirm_label, "Login");

  // Khởi tạo bàn phím ảo (ẩn ban đầu)
  keyboard = lv_keyboard_create(screen1, NULL);
  lv_obj_align(keyboard, NULL, LV_ALIGN_IN_TOP_LEFT, -2, 111);
  lv_obj_set_size(keyboard, 480, 96);
  lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);  // Ẩn bàn phím
}

// Hàm vẽ màn hình TFT cho LVGL
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, area->x2, area->y2);
  tft.pushColors((uint16_t*)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// Callback cho Button 1 (khi bấm vào)
void btn1_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    // Khi bấm nút, hiện Text Area và bàn phím
    lv_obj_clear_flag(txt_area, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(password_txt_area, LV_OBJ_FLAG_HIDDEN);  // Hiện Text Area mật khẩu
  }
}

// Callback cho nút Xác nhận mật khẩu
void btn_confirm_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    const char *password = lv_textarea_get_text(password_txt_area);
    
    // Kiểm tra mật khẩu
    if (strcmp(password, valid_password) == 0) {
      // Nếu mật khẩu đúng, chuyển sang màn hình 2
      lv_obj_t *screen2 = lv_obj_create(NULL, NULL);
      lv_scr_load(screen2);  // Chuyển sang màn hình 2

      // Tạo một label trên màn hình 2
      lv_obj_t *label = lv_label_create(screen2, NULL);
      lv_label_set_text(label, "Welcome to Screen 2!");
      lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    } else {
      // Nếu mật khẩu sai, hiện thông báo lỗi
      lv_obj_t *error_label = lv_label_create(lv_scr_act(), NULL);
      lv_label_set_text(error_label, "Incorrect Password!");
      lv_obj_align(error_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
    }
  }
}

void loop() {
  lv_task_handler();  // Xử lý các nhiệm vụ của LVGL
  delay(5);  // Giảm độ trễ
}
