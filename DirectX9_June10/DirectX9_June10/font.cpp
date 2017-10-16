#include "font.h"

void font::Initialize() {
	tex.LoadGraph(_T("font.png"));
	sprite.SetTexture(&tex);
	sprite.SetSpriteSize(32, 32);
	strcpy_s(message, 128, "takatsu yuuta.");

}

void font::Update() {
	int i = 0;
	while (message[i] != '\0') {

		sprite.SetFrame(message[i]);
		sprite.Draw(50.0f + (float)(i * 32), 100.0f);
		i++;
	}
}