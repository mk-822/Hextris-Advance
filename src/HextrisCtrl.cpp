// HextrisCtrl.cpp: HextrisCtrl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "HextrisCtrl.h"
#include "debug.h"

enum PHASE{
	PHASE_APPEAR,
	PHASE_MOVING,
	PHASE_GROUNDED,
	PHASE_FIXED,
	PHASE_ERASED
};

enum SPECIALDRAW{
	BLOCK_FIX_EFFECT = 1,
};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

HextrisCtrl::HextrisCtrl(){
}

void HextrisCtrl::Initialize(draw* Dxg,Image* Image,JoyPadCtrl* Input,DataFileLoader* BlockData){
	dxg = Dxg;
	image = Image;
	input = Input;
	blockData = BlockData;
	count = 0;
	phase = 0;
	eraseData.Clear();

	queErace = 0;
	queFix = 0;
	for(int i=0 ; i<FLOORUP_MAX ; i++){
		queFloorUp[i] = -1;
	}

	nextBlock = GenerateNext(10);
	effectFlag = 0;
	wait = 0;

	curBlock.center_x = 0;
	curBlock.center_y = 0;
	curBlock.color = 0;
	for(int i=0 ; i<4 ; i++)
		curBlock.pos[i] = 0;

	hField.Initialize();
	for(int i=0; i<22; i++){
		hField.SetN(1,i,11);
		hField.SetN(15,i,11);
	}
	for(int j=2; j<15; j++){
		hField.SetN(j,0,11);
		hField.SetN(j,22,11);
	}
}

// ■■■■■■■■■■■■■■■■■■■メイン■■■■■■■■■■■■■■■■■■
int HextrisCtrl::Main(int player){
	if(wait){
		wait--;
	}else{
		switch(phase){
////// 出現処理 ////////////
		case PHASE_APPEAR:
			if(queFloorUp[0] != -1){	// せり上がり
				FloorUp(DequeFloorUp());
				break;
			}

			if(input->GetKeyState(player,0) & (BUTTON[0] | BUTTON[2])){	//左回転
				NextToCurrent(-1);
				Sound::PlaySe(8);
			}else if(input->GetKeyState(player,0) & BUTTON[1]){	//右回転
				NextToCurrent(1);
				Sound::PlaySe(8);
			}else{
				NextToCurrent(0);
			}

			nextBlock = GenerateNext(10);
			fallcount = delayData.fall;
			fixcount = delayData.fix;
			forcefixcount = 0;

			if(IsGrounded()){
				ShiftPhase(2);
			}else{
				ShiftPhase(1);
			}

			if(IsDead()){
				return true;
			}

			if(fallcount){
				fallcount--;
			}else{
				BlockMove(0,delayData.g);
				fallcount = delayData.fall;
				fixcount = delayData.fix;
			}
			break;
////// ブロック移動中 ///////
		case PHASE_MOVING:
			if(IsGrounded()){
				ShiftPhase(1);
			}

			if(fallcount){
				fallcount--;
			}else{
				BlockMove(0,delayData.g);
				fallcount = delayData.fall;
				fixcount = delayData.fix;
			}

			//操作による移動
			if(input->GetKeyState(player,2) & UP){	//一気に落下
				BlockMove(0,20);
			}
			if(input->GetKeyState(player,1) & LEFT){	//左に移動
				BlockMove(-1,0);
			}
			if(input->GetKeyState(player,1) & RIGHT){//右に移動
				BlockMove(1,0);
			}
			if(input->GetKeyState(player,0) & DOWN){	//一段落下
				BlockMove(0,1);
			}
			//回転
			if(input->GetKeyState(player,2) & (BUTTON[0] | BUTTON[2])){	//左回転
				BlockSpin(-1);
				Sound::PlaySe(8);
			}
			if(input->GetKeyState(player,2) & BUTTON[1]){	//右回転
				BlockSpin(1);
				Sound::PlaySe(8);
			}
			break;
////// 移動中だが接地している /////
		case PHASE_GROUNDED:
			if(fixcount){
				fixcount--;
			}else{
				BlockFix();
				ShiftPhase(1);
				break;
			}

			// うろちょろしてると強制固定
			if(forcefixcount >= 30){
				BlockFix();
				ShiftPhase(1);
				break;
			}

			//接地中だったら固定
			if(input->GetKeyState(player,0) & DOWN){
				BlockFix();
				ShiftPhase(1);
				break;
			}
			if(input->GetKeyState(player,1) & LEFT){	//左に移動
				BlockMove(-1,0);
				forcefixcount++;
			}
			if(input->GetKeyState(player,1) & RIGHT){//右に移動
				BlockMove(1,0);
				forcefixcount++;
			}

			//回転
			if(input->GetKeyState(player,2) & (BUTTON[0] | BUTTON[2])){	//左回転
				BlockSpin(-1);
				Sound::PlaySe(8);
				forcefixcount++;
			}
			if(input->GetKeyState(player,2) & BUTTON[1]){	//右回転
				BlockSpin(1);
				Sound::PlaySe(8);
				forcefixcount++;
			}

			if(!IsGrounded()){
				if(fallcount){
					fallcount--;
				}else{
					BlockMove(0,delayData.g);
					fallcount = delayData.fall;
					fixcount = delayData.fix;
				}
				ShiftPhase(-1);
			}
			break;
////// 固定後の処理 /////////
		case PHASE_FIXED:
			Sound::PlaySe(5);
			effectFlag |= BLOCK_FIX_EFFECT;
			queFix = 1;
			if(queErace = BlockErase()){
				ShiftPhase(1);
				if(queErace==4){
					Sound::PlaySe(3);
				}else{
					Sound::PlaySe(4);
				}
			}else{
				ShiftPhase(2);
			}
			break;
		case PHASE_ERASED:
			Sound::PlaySe(0);
			for(int i=eraseData.GetFreq() ; i>0 ; i--){
				for(int j=eraseData.GetLine() ; j>=0 ; j--){
					for(int k=0 ; k<13 ; k++){
						if(j==0){
							hField.SetField(k,j,0);
						}else{
							hField.SetField(k,j,hField.GetField(k,j-1));
						}
					}
				}
			}
			eraseData.Clear();
			ShiftPhase(1);
			break;
		}
	}
	count++;
	return false;
}
//■■■■■■■■■■■■■■■■■■メイン終わり■■■■■■■■■■■■■■■■■■

void HextrisCtrl::Draw(HexFieldDrawData* drawData){
	// 背景を暗くするため
	dxg->TexturePos(0,0,80,160);
	dxg->Draw(
		image->i[BLANK_IMG],
		(float)(drawData->game_pos_x + GAME_POS_OFFSET_X),
		(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + BLOCK_OFFSET_Y),
		true,
		128
	);

	// フィールド配列の枠の描画 *直接指定
	for(int i=0; i<40; i++){
		for(int j=0; j<13; j++){
			if(hField.Get(j,i)){
				dxg->ColorChange(ADD_BLEND);
				dxg->TexturePos( 32+(i/2%4)*16 ,88,9,10);
				dxg->Draw(
					image->i[BLOCK_IMG],
					(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + j*BLOCK_OFFSET_X -1),
					(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + i*BLOCK_OFFSET_Y -1),
					false
				);
			}
		}
	}
	
	// フィールド配列の描画
	for(int i=0; i<40; i++){
		for(int j=0; j<13; j++){
			dxg->TexturePos(
				hField.Get(j,i) * BLOCK_SIZE_X,
				BLOCK_IMAGE_OFFSET_Y + BLOCK_SIZE_Y * 6,
				BLOCK_SIZE_X,
				BLOCK_SIZE_Y
			);
			dxg->Draw(
				image->i[BLOCK_IMG],
				(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + j*BLOCK_OFFSET_X),
				(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + i*BLOCK_OFFSET_Y)
			);
			if(hField.Get(j,i)){
				dxg->ColorChange(ADD_BLEND);
				dxg->TexturePos(
					(14 + j%2) * BLOCK_SIZE_X,
					((count / 4) % 8) * BLOCK_SIZE_Y + BLOCK_IMAGE_OFFSET_Y,
					BLOCK_SIZE_X,
					BLOCK_SIZE_Y
				);
				dxg->Draw(
					image->i[BLOCK_IMG],
					(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + j*BLOCK_OFFSET_X),
					(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + i*BLOCK_OFFSET_Y),
					false
				);
			}
		}
	}

	// ネクストの描画
	dxg->TexturePos(NEXT_SIZE_X * (nextBlock + 1),NEXT_IMAGE_OFFSET_Y,NEXT_SIZE_X,NEXT_SIZE_Y);
	dxg->Draw(image->i[BLOCK_IMG],(float)(drawData->game_pos_x + NEXT_OFFSET_X),(float)(drawData->game_pos_y + NEXT_OFFSET_Y));

	if(phase & (PHASE_MOVING | PHASE_GROUNDED)){
		// カレント点滅の描画 *直接指定
		for(int i=0 ; i<4 ; i++){
			if(count % 2){
				dxg->ColorChange(ADD_BLEND);
				dxg->TexturePos(0,88,16,16);
				dxg->Draw(
					image->i[BLOCK_IMG],
					(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + (blockData->posData[curBlock.pos[i]].x + curBlock.center_x) * BLOCK_OFFSET_X - 4),
					(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + (blockData->posData[curBlock.pos[i]].y + curBlock.center_y) * BLOCK_OFFSET_Y - 4),
					false
				);
			}
		}
	
		// カレントの描画
		for(int i=0 ; i<4 ; i++){
			dxg->TexturePos(
				curBlock.color * BLOCK_SIZE_X,
				BLOCK_IMAGE_OFFSET_Y,
				BLOCK_SIZE_X,
				BLOCK_SIZE_Y
			);
			dxg->Draw(
				image->i[BLOCK_IMG],
				(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + (blockData->posData[curBlock.pos[i]].x + curBlock.center_x) * BLOCK_OFFSET_X),
				(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + (blockData->posData[curBlock.pos[i]].y + curBlock.center_y) * BLOCK_OFFSET_Y)
			);
		}
	}

	// フラッシュの描画
	if(effectFlag & BLOCK_FIX_EFFECT){
		for(int i=0 ; i<4 ; i++){
			dxg->TexturePos(
				13 * BLOCK_SIZE_X,
				BLOCK_IMAGE_OFFSET_Y,
				BLOCK_SIZE_X,
				BLOCK_SIZE_Y
			);
			dxg->Draw(
				image->i[BLOCK_IMG],
				(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + (blockData->posData[curBlock.pos[i]].x + curBlock.center_x) * BLOCK_OFFSET_X),
				(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + (blockData->posData[curBlock.pos[i]].y + curBlock.center_y) * BLOCK_OFFSET_Y)
			);
		}
		effectFlag &= ~BLOCK_FIX_EFFECT;
	}

	// ガイドの表示
	if(phase & PHASE_MOVING){
		int center_x_bak = curBlock.center_x;
		int center_y_bak = curBlock.center_y;
		BlockMove(0,20);

		for(int i=0 ; i<4 ; i++){
			dxg->TexturePos(
				curBlock.color * BLOCK_SIZE_X,
				BLOCK_IMAGE_OFFSET_Y,
				BLOCK_SIZE_X,
				BLOCK_SIZE_Y
			);
			dxg->Draw(
				image->i[BLOCK_IMG],
				(float)(drawData->game_pos_x + GAME_POS_OFFSET_X + (blockData->posData[curBlock.pos[i]].x + curBlock.center_x) * BLOCK_OFFSET_X),
				(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + (blockData->posData[curBlock.pos[i]].y + curBlock.center_y) * BLOCK_OFFSET_Y),
				true,
				96
			);
		}

		curBlock.center_x = center_x_bak;
		curBlock.center_y = center_y_bak;
	}

	// 壁の描画
	dxg->TexturePos(0,0,96,176);
	dxg->Draw(image->i[FRAME_IMG],(float)(drawData->game_pos_x),(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y - BLOCK_OFFSET_Y));

	// 消去エフェクト
	for(int i=0 ; i<4 ; i++){
		if(eraseEffectData[i].effecttime%2){
			for(int j=0 ; j<13 ; j++){
				dxg->ColorChange(ADD_BLEND);
				dxg->TexturePos(0,88,16,16);
				dxg->Draw(
					image->i[BLOCK_IMG],
					drawData->game_pos_x + eraseEffectData[i].x[j] -4,
					drawData->game_pos_y + eraseEffectData[i].y[j] -4,
					false,
					eraseEffectData[i].effecttime*255 / eraseEffectData[i].EFFECTTIME
				);
			}
		}
	}
	for(int i=0 ; i<4 ; i++){
		if(eraseEffectData[i].effecttime){
			eraseEffectData[i].effecttime--;
			for(int j=0 ; j<13 ; j++){
				dxg->TexturePos(
					eraseEffectData[i].color[j] * BLOCK_SIZE_X,
					((count/eraseEffectData[i].animespeed[j])%6)*BLOCK_SIZE_Y + BLOCK_IMAGE_OFFSET_Y,
					BLOCK_SIZE_X,
					BLOCK_SIZE_Y
				);
				dxg->Draw(
					image->i[BLOCK_IMG],
					drawData->game_pos_x + eraseEffectData[i].x[j],
					drawData->game_pos_y + eraseEffectData[i].y[j]
				);
				eraseEffectData[i].x[j] += eraseEffectData[i].movex[j];
				eraseEffectData[i].y[j] += eraseEffectData[i].movey[j];
				eraseEffectData[i].movey[j] += eraseEffectData[i].moveymove[j];
				eraseEffectData[i].moveymove[j] *= eraseEffectData[i].gravity;
			}
		}
	}
}

void HextrisCtrl::ChangeLevel(DelayData *newLevel)
{
	delayData = *newLevel;
}

int HextrisCtrl::NextToCurrent(int rotation)
{
	curBlock.color = nextBlock + 1;
	curBlock.center_x = blockData->blockData[nextBlock].center_x;
	curBlock.center_y = blockData->blockData[nextBlock].center_y;
	for(int i=0 ; i<4 ; i++)
		curBlock.pos[i] = blockData->blockData[nextBlock].pos[i];

	BlockSpin(rotation);
	GenerateNext(10);

	return 0;
}// 返り値が 1 だったら手詰まり

int HextrisCtrl::GenerateNext(int max)
{
	return rand.Decision();
}

int HextrisCtrl::BlockMove(int x, int y)
{
	// まずは絶対値を算出
	int abs_x = x; int abs_y = y;
	if(abs_x<0)
		abs_x *= -1;
	if(abs_y<0)
		abs_y *= -1;

	int adjust_y = 1;
	if(curBlock.center_x % 2)
		adjust_y = -1;

	// 動けた回数
	int movecount = 0;
	// 動けなかったらオンになるフラグ（ズレて動けるか試す）
	bool movefailed = false;

	// 処理を強制分岐(x 軸 y 軸を同時に動かすことはできない)
	if(abs_x){
		for(int i=0 ; i<abs_x ; i++){
			for(int j=0 ; j<=4 ; j++){
				if(j==4){
					curBlock.center_x += abs_x / x;
					curBlock.center_y += adjust_y;
					adjust_y = 1;
					if(curBlock.center_x % 2)
						adjust_y = -1;
					movecount++;
					break;
				}
				if(
					hField.Get(
						blockData->posData[curBlock.pos[j]].x + curBlock.center_x + abs_x / x,
						blockData->posData[curBlock.pos[j]].y + curBlock.center_y + adjust_y
					)
				){
					if(movefailed){
						movefailed = false;
					}else{
						movefailed = true;
						adjust_y *= -1;
						i--;
					}
					break;
				}
			}
		}
	}else if(abs_y){
		for(int i=0 ; i<abs_y ; i++){
			for(int j=0 ; j<=4 ; j++){
				if(j==4){
					curBlock.center_y += abs_y / y * 2;
					movecount++;
					break;
				}
				if(
					hField.Get(
						blockData->posData[curBlock.pos[j]].x + curBlock.center_x,
						blockData->posData[curBlock.pos[j]].y + curBlock.center_y + abs_y / y * 2
					)
				){
					break;
				}
			}
		}
	}
	return movecount;
}

int HextrisCtrl::BlockSpin(int spin)
{
	// 回転が成功したかどうか
	int spincount=0;
	int adjust_x , adjust_y;

	if(spin){
		for(int k=0 ; k<=7 ; k++){
			switch(k){
			case 0:	//まずは普通に
				adjust_x = 0; adjust_y = 0;
				break;
			case 1: //下だ
				adjust_x = 0; adjust_y = 2;
				break;
			case 2:	//それで駄目なら…左！
				adjust_x = -1; adjust_y = AdjustX();
				break;
			case 3: //くっ…右！
				adjust_x = 1; adjust_y = AdjustX();
				break;
			case 4: //左下
				adjust_x = -1; adjust_y = 2 + AdjustX();
				break;
			case 5: //右下
				adjust_x = 1; adjust_y = 2 + AdjustX();
				break;
			case 6:	//下下
				adjust_x = 0; adjust_y = 4;
				break;
			case 7: //うおお！あとは上しかねえ！
				adjust_x = 0; adjust_y = -2;
				break;
			}
			if(spincount){
				break;
			}
			for(int i=0 ; i<=4 ; i++){
				if(i==4){
					for(int j=0 ; j<4 ; j++){
						if(spin == -1){
							curBlock.pos[j] = blockData->posData[curBlock.pos[j]].next;
						}else{
							curBlock.pos[j] = blockData->posData[curBlock.pos[j]].previous;
						}
					}
					curBlock.center_x += adjust_x;
					curBlock.center_y += adjust_y;
					spincount++;
					break;
				}
				if(spin == -1){
					if(
						hField.Get(
							blockData->posData[ blockData->posData[curBlock.pos[i]].next ].x + curBlock.center_x + adjust_x,
							blockData->posData[ blockData->posData[curBlock.pos[i]].next ].y + curBlock.center_y + adjust_y
						)
					){
						break;
					}
				}else{
					if(
						hField.Get(
							blockData->posData[ blockData->posData[curBlock.pos[i]].previous ].x + curBlock.center_x + adjust_x,
							blockData->posData[ blockData->posData[curBlock.pos[i]].previous ].y + curBlock.center_y + adjust_y
						)
					){
						break;
					}
				}
			}
		}
	}
	return spincount;
}

int HextrisCtrl::IsGrounded()
{
	for(int i=0 ; i<=4 ; i++){
		if(i==4){
			return 0;
			break;
		}
		if(
			hField.Get(
				blockData->posData[curBlock.pos[i]].x + curBlock.center_x,
				blockData->posData[curBlock.pos[i]].y + curBlock.center_y + 2
			)
		){
			break;
		}
	}
	return 1;
}

int HextrisCtrl::BlockFix()
{
	for(int i=0 ; i<4 ; i++){
		hField.Set(
			blockData->posData[curBlock.pos[i]].x + curBlock.center_x,
			blockData->posData[curBlock.pos[i]].y + curBlock.center_y,
			curBlock.color
		);
	}

	return 0;
}

int HextrisCtrl::BlockErase()
{
	int erasecount = 0;
	for(int i=19 ; i>=0 ; i--){
		for(int j=0 ; j<=13 ; j++){
			if(j == 13){
				for(int k=0 ; k<13 ; k++){
			// エフェクトへのデータ渡し ///////////////
					eraseEffectData[erasecount].GenerateMove();
					eraseEffectData[erasecount].color[k] = hField.GetField(k,i);
					eraseEffectData[erasecount].x[k] = (float)(GAME_POS_OFFSET_X + k*BLOCK_OFFSET_X);
					eraseEffectData[erasecount].y[k] = (float)(GAME_POS_OFFSET_Y + i*BLOCK_OFFSET_Y*2);
			///////////////////////////////////////////
					hField.SetField(k,i,0);
				}
				erasecount++;
				eraseData.SetLine(i);
				break;
			}
			if(!hField.GetField(j,i)){
				break;
			}
		}
	}
	return erasecount;
}

int HextrisCtrl::ShiftPhase(int diff)
{
	phase += diff;
	if(phase > PHASE_ERASED){
		phase = PHASE_APPEAR;
	}
	switch(phase){
	case PHASE_APPEAR:
		wait = delayData.appear;
		break;
	case PHASE_ERASED:
		wait = delayData.erase;
		break;
	}
	return phase;
}

int HextrisCtrl::AdjustX(int x)
{
	if(x != -256){
		if(x % 2){
			return -1;
		}else{
			return 1;
		}
	}
	if(curBlock.center_x % 2){
		return -1;
	}else{
		return 1;
	}
}

void HextrisCtrl::DrawField(HexFieldDrawData* drawData, int trans, bool shadow)
{
	// 背景を暗くするため
	if(shadow){
		dxg->TexturePos(0,0,80,160);
		dxg->Draw(
			image->i[BLANK_IMG],
			(float)(drawData->game_pos_x + GAME_POS_OFFSET_X),
			(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y + BLOCK_OFFSET_Y),
			true,
			trans/2
		);
		// ネクストの描画
		dxg->TexturePos(NEXT_SIZE_X * (nextBlock + 1),NEXT_IMAGE_OFFSET_Y,NEXT_SIZE_X,NEXT_SIZE_Y);
		dxg->Draw(
			image->i[BLOCK_IMG],
			(float)(drawData->game_pos_x + NEXT_OFFSET_X),
			(float)(drawData->game_pos_y + NEXT_OFFSET_Y),
			true,
			trans
		);
	}
	// 壁の描画
	dxg->TexturePos(0,0,96,176);
	dxg->Draw(image->i[FRAME_IMG],(float)(drawData->game_pos_x),(float)(drawData->game_pos_y + GAME_POS_OFFSET_Y - BLOCK_OFFSET_Y),true,trans);
}

int HextrisCtrl::IsDead()
{
	for(int i=0 ; i<=4 ; i++){
		if(i==4){
			return 0;
			break;
		}
		if(
			hField.Get(
				blockData->posData[curBlock.pos[i]].x + curBlock.center_x,
				blockData->posData[curBlock.pos[i]].y + curBlock.center_y
			)
		){
			break;
		}
	}
	return 1;
}

void HextrisCtrl::CountUp()	// 力業でゴメン
{
	count++;
}

void HextrisCtrl::EnqueFloorUp(int up, int color)
{
	for(int i=0 ; i<FLOORUP_MAX ; i++){
		if(!up){
			break;
		}
		if(queFloorUp[i] == -1){
			queFloorUp[i] = color;
			up--;
		}
	}
}

int HextrisCtrl::DequeFloorUp()
{
	int tmp = queFloorUp[0];
	for(int i=0 ; i<FLOORUP_MAX-1 ; i++){
		queFloorUp[i] = queFloorUp[i+1];
	}
	queFloorUp[FLOORUP_MAX-1] = -1;
	return tmp;
}

void HextrisCtrl::FloorUp(int color)
{
	for(int i=0 ; i<19 ; i++){
		for(int j=0 ; j<13 ; j++){
			hField.SetField(j,i,hField.GetField(j,i+1));
		}
	}
	for(int i=0 ; i<13 ; i++){
		if(hField.GetField(i,18)){
			hField.SetField(i,19,color);
		}
	}
}
