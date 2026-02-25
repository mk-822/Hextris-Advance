#ifndef MAINHEADER
#define MAINHEADER

// クライアント領域のサイズに合わせてウィンドウをリサイズする関数
LRESULT SetWindowSizeClient(HWND hwnd,WORD width,WORD height,int resX,int resY){
    RECT    WindowRect;
    int     delta_width;
    int     delta_height;

    GetWindowRect(hwnd,&WindowRect);
    delta_width=(WindowRect.right-WindowRect.left)-width;
    delta_height=(WindowRect.bottom-WindowRect.top)-height;
    MoveWindow(hwnd,WindowRect.left,WindowRect.top,resX+delta_width,resY+delta_height,TRUE);

    return  0;
}

#endif