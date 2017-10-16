#include "SpriteTest.h"



SpriteTest::SpriteTest() {
	x = 320.0f - 16.0f;
	y = 240.0f - 16.0f;
	mx = my = 2.0f;
}

void SpriteTest::Initialize() {
	tex.LoadGraph(_T("soccerball.png"));
	sprite.SetTexture(&tex);
}

void SpriteTest::Update() {
	x += mx;
	if (x < 0) {
		x = 0;
		mx = -mx;
	}
	else if (x > 640 - 32) {
		x = 640 - 32;
		mx = -mx;
	}
		y += my;
	if (y < 0) {
		y = 0;
		my = -my;
	}
	else if (y > 480 - 32) {
		y = 480 - 32;
		my = -my;
	}
	sprite.Draw(x, y);
}