#include "Animate.h"

void CAnimationTest::Initialize() {
	tex.LoadGraph(_T("anime.png"));
	sprite.SetTexture(&tex);
	sprite.SetSpriteSize(128, 128);
}

void CAnimationTest::Update() {
	static int frame = 0, count = 0;
	if (++count >= 12) {
		count = 0;
		if (++frame >= 8) frame = 0;
		sprite.SetFrame(frame);
	}

	sprite.Draw(320.0f, 240.0f);
}