#include "unihiker_k10.h"
#include "AIRecognition.h"

UNIHIKER_K10 k10;
AIRecognition ai;

uint8_t screen_dir = 2;
String lastDecodedText = "";

void configureCameraForQr() {
    sensor_t *sensor = esp_camera_sensor_get();
    if (sensor == nullptr) {
        return;
    }

    sensor->set_contrast(sensor, 2);
    sensor->set_sharpness(sensor, 2);
    sensor->set_denoise(sensor, 1);
    sensor->set_bpc(sensor, 1);
    sensor->set_wpc(sensor, 1);
    sensor->set_lenc(sensor, 1);
    sensor->set_whitebal(sensor, 1);
    sensor->set_awb_gain(sensor, 1);
    sensor->set_gain_ctrl(sensor, 1);
    sensor->set_exposure_ctrl(sensor, 1);
    sensor->set_aec2(sensor, 1);
}

void showDecodedText(const String &text) {
    k10.canvas->canvasRectangle(0, 248, 240, 72, 0xFFFFFF, 0xFFFFFF, true);
    k10.canvas->canvasText(text, 6, 252, 0x000000, k10.canvas->eCNAndENFont16, 18, false);
    k10.canvas->updateCanvas();
}

void setup() {
    k10.begin();
    k10.initScreen(screen_dir);

    ai.initAi();
    k10.initBgCamerImage();
    configureCameraForQr();
    k10.setBgCamerImage(false);
    k10.creatCanvas();

    ai.switchAiMode(ai.NoMode);
    k10.setBgCamerImage(true);
    ai.switchAiMode(ai.Code);

    showDecodedText("Decoded: ");
}

void loop() {
    if (ai.isDetectContent(AIRecognition::Code)) {
        String decodedText = ai.getQrCodeContent();
        if (decodedText.length() > 0 && decodedText != lastDecodedText) {
            lastDecodedText = decodedText;
            showDecodedText("Decoded: " + decodedText + ".");
        }
    }

    delay(100);
}
