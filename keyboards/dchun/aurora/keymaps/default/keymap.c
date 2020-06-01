#include QMK_KEYBOARD_H

#include "app_ble_func.h"
#include "nrf_log.h"
#include "nrf_gpio.h"

enum layer_number {
  _QWERTY = 0,
  _ADJUST
};

enum custom_keycodes {
  AD_WO_LUST = SAFE_RANGE, /* Start advertising without whitelist  */
  BLE_DIS,                 /* Disable BLE HID sending              */
  BLE_EN,                  /* Enable BLE HID sending               */
  USB_DIS,                 /* Disable USB HID sending              */
  USB_EN,                  /* Enable USB HID sending               */
  DELBNDS,                 /* Delete all bonding                   */
  ADV_ID0,                 /* Start advertising to PeerID 0        */
  ADV_ID1,                 /* Start advertising to PeerID 1        */
  ADV_ID2,                 /* Start advertising to PeerID 2        */
  ADV_ID3,                 /* Start advertising to PeerID 3        */
  ADV_ID4,                 /* Start advertising to PeerID 4        */
  BATT_LV,                 /* Display battery level in milli volts */
  DEL_ID0,                 /* Delete bonding of PeerID 0           */
  DEL_ID1,                 /* Delete bonding of PeerID 1           */
  DEL_ID2,                 /* Delete bonding of PeerID 2           */
  DEL_ID3,                 /* Delete bonding of PeerID 3           */
  DEL_ID4,                 /* Delete bonding of PeerID 4           */
  ENT_DFU,                 /* Start bootloader                     */
  ENT_SLP,                 /* Deep sleep mode                      */
};

// Layer Mode aliases
#define KC_ADJ   MO(_ADJUST)

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_KANJI KANJI

#define KC_DEBUG DEBUG

#define KC_RST   ENT_DFU
#define KC_BATT  BATT_LV
#define KC_BTON  BLE_EN
#define KC_BTOF  BLE_DIS
#define KC_USBON USB_EN
#define KC_USBOF USB_DIS
#define KC_BTNEW AD_WO_L
#define KC_BTID0 ADV_ID0
#define KC_BTID1 ADV_ID1
#define KC_BTID2 ADV_ID2
#define KC_BTID3 ADV_ID3
#define KC_BTID4 ADV_ID4



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc | 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |~ ` |
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |Del |
   * |----------------------------------------------------------------|
   * |CAPS   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |PgUp|
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|PgDn|
   * |----------------------------------------------------------------|
   * |Ctrl|Win |Alt |        Space          |Alt| FN|Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
  [_QWERTY] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME, \
    KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP, \
    KC_CAPS, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN, \
    KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,           KC_UP,   KC_END, \
    KC_LCTL, KC_LGUI, KC_LALT,                        KC_SPC,                  KC_ADJ,  KC_RALT,          KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [_ADJUST] = LAYOUT(
     RESET, ADV_ID1, ADV_ID2, DEL_ID1, DEL_ID2,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______, ENT_DFU,  ______, \
     DEBUG, DELBNDS, USB_DIS,  USB_EN,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______, \
   AD_WO_L,  ______, BLE_DIS,  BLE_EN,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,          ENT_SLP,  ______, \
    ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,           ______,  ______, \
    ______,  ______,  ______,                             ______,                    ______,  ______,           ______,  ______,  ______
  )
};


// uint32_t layer_state_set_user(uint32_t state) {
//   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }

// adafruit bootloader
#define DFU_DBL_RESET_MAGIC             0x5A1AD5
#define DFU_DBL_RESET_MEM               0x20007F7C
#define DFU_MAGIC_UF2_RESET             0x57
uint32_t* dbl_reset_mem = ((uint32_t*)  DFU_DBL_RESET_MEM );
void uf2_jump(void) {
  NRF_POWER->GPREGRET = DFU_MAGIC_UF2_RESET;
  *dbl_reset_mem = DFU_DBL_RESET_MAGIC;
  NRF_POWER->RESETREAS |= POWER_RESETREAS_RESETPIN_Msk;
  NVIC_SystemReset();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  char str[16];
  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      return false;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      return false;
    case USB_EN:
      set_usb_enabled(true);
      return false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      return false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      return false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      return false;
      break;
    case ADV_ID0:
      NRF_LOG_INFO("Restart advertising ID: %d", 0);
      restart_advertising_id(0);
      return false;
    case ADV_ID1:
      NRF_LOG_INFO("Restart advertising ID: %d", 1);
      restart_advertising_id(1);
      return false;
    case ADV_ID2:
      NRF_LOG_INFO("Restart advertising ID: %d", 2);
      restart_advertising_id(2);
      return false;
    case ADV_ID3:
      NRF_LOG_INFO("Restart advertising ID: %d", 3);
      restart_advertising_id(3);
      return false;
    case ADV_ID4:
      restart_advertising_id(4);
      return false;
    case DEL_ID0:
      delete_bond_id(0);
      return false;
    case DEL_ID1:
      delete_bond_id(1);
      return false;
    case DEL_ID2:
      delete_bond_id(2);
      return false;
    case DEL_ID3:
      delete_bond_id(3);
      return false;
    case BATT_LV:
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      return false;
    case ENT_DFU: case RESET:
      bootloader_jump();
      return false;
    case DEBUG:
      uf2_jump();
      return false;
    case BLE_DBG:
      set_ble_enabled(true);
      //delete_bonds(); // causes reset
      restart_advertising_wo_whitelist();
      return false;
    case BL_INC:
      nrf_gpio_pin_set(BACKLIGHT_PIN);
      return false;
    case BL_DEC:
      nrf_gpio_pin_clear(BACKLIGHT_PIN);
      return false;
    }
  }
  else if (!record->event.pressed) {
    switch (keycode) {
    case ENT_SLP:
      sleep_mode_enter();
      return false;
    }

  }
  return true;
};