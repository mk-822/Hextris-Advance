# Hextris Advance (Butano / GBA)

このディレクトリには、既存の Windows 版 Hextris をベースにした GBA 向け移植実装が入っています。

## ビルド

リポジトリのルートで以下を実行します:

```bash
make
```

成功すると `hextris_advance_gba.gba` が生成されます。

## 実装メモ

- Butano ライブラリを使った C++ 実装です。
- BGM (mp3) は GBA 制約のためオミットしています。
- ゲームルールは「横一列が埋まると消去」ベースで実装しています。
- 操作:
  - `LEFT` / `RIGHT`: 移動
  - `A`: 色ローテーション
  - `DOWN`: 高速落下
  - `START`: ゲームオーバー後の再開
