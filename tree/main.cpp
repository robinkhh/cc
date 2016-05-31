#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <string>
#include <iostream>
using namespace std;


HINSTANCE hInst;
string PreOrderStr("");
int level=0;
int rootx=150;
int rooty=175;

typedef struct _bxy{
int bx;
int by;
}bxy;
bxy ball_xy[10];

typedef struct _bt{
    int ball_num;
    int ball_x;
    int ball_y;
    int level;
    _bt* left;
    _bt* right;
}bt;
bt* anode;
//==================================================================================================
//FUNCTION: GetText
//==================================================================================================
void GetText(HWND hCtrl,HWND hwndDlg) {
      int len = GetWindowTextLength( hCtrl );
      if(len > 0)  {
        char* buf;
              //buf = (char*) GlobalAlloc(GPTR, len + 1);
                buf = (char*)malloc(sizeof(len+1));
        GetWindowText (hCtrl, buf, len + 1);
        MessageBox    (NULL, buf, "YOU ENTERED:", MB_ICONERROR | MB_OK | MB_TOPMOST);
        HWND myResult = GetDlgItem(hwndDlg, IDC_RESULT);               //Get Control Handle.
        SetWindowText(myResult, buf);
        //GlobalFree    ((HANDLE)buf);
        free(buf);
      }
    }

//-------------------------------------------------
void insertData(bt** A ,int insdata){
char strMessage[3];
itoa(insdata,strMessage,10);
  //MessageBox (NULL, strMessage, "insertData:", MB_ICONERROR | MB_OK | MB_TOPMOST);
    if (*A==NULL){
        bt* root;
        root=(bt*)malloc(sizeof(bt));
        root->ball_num=insdata;
        root->ball_x=rootx;
        root->ball_y=rooty;
        root->level=level;
        //ball_xy[0] array start with [0]
        ball_xy[insdata-1].bx=rootx;
        ball_xy[insdata-1].by=rooty+(level*45);
        root->left=NULL;
        root->right=NULL;
        *A=root;
        }
    else if (insdata < (*A)->ball_num ){
        level++;

        rootx=(*A)->ball_x - 40;
         if (level==1){rootx=(*A)->ball_x - 75;}
        insertData(&((*A)->left) , insdata);
    }
    else if ((*A)->ball_num < insdata ){
        level++;

        rootx=(*A)->ball_x +40;
        if (level==1){rootx=(*A)->ball_x + 75;}
        insertData(&((*A)->right) , insdata);
    }

}

void PreOrder(bt* A,HWND hwndDlg){
 char listTmp[3];

 if(A!=NULL){

    itoa(A->ball_num,listTmp,10);

    PreOrderStr.append(listTmp);
    //MessageBox (NULL, PreOrderStr.c_str(), "PreOrderStr:", MB_ICONERROR | MB_OK | MB_TOPMOST);
    //
    HWND myResult = GetDlgItem(hwndDlg, IDC_RESULT);               //Get Control Handle.
    SetWindowText(myResult, PreOrderStr.c_str());
    PreOrder(&(*(A->left)),hwndDlg);
    PreOrder(&(*(A->right)),hwndDlg);

 }

}


//==================================================================================================
//FUNCTION: DlgProc
//==================================================================================================
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
     //binary tree initial
     anode=0;
     //anode->ball_name=(char*)"test str
     //SET TEXT.
     HWND hCtrl = GetDlgItem(hwndDlg, IDC_EDIT);               //Get Control Handle.
     SetWindowText(hCtrl, "0");              //Set Text.

   }return TRUE;
    case WM_PAINT:
     {

      PAINTSTRUCT 	ps;
      HDC 			hdc;
      BITMAP 		bitmap;
      HDC 			hdcMem;
      HGDIOBJ 		oldBitmap;
      string        ball_str1("bmp\\");
      int           num;
      string        ball_str2(".bmp");

    hdc = BeginPaint(hwndDlg, &ps);
    hdcMem = CreateCompatibleDC(hdc);
int countPreOrderStr=PreOrderStr.size();
    for (num=0; num<countPreOrderStr; num++){

        ball_str1+=(PreOrderStr[num]);
        ball_str1+=ball_str2;
   //MessageBox(hwndDlg, ball_str1.c_str(), "ball_num", MB_OK | MB_ICONEXCLAMATION);

        HBITMAP hBitmap = (HBITMAP)LoadImage(hInst,
                                         ball_str1.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(hBitmap==NULL){
       MessageBox(hwndDlg, "no bitmap", "ball_num", MB_OK | MB_ICONEXCLAMATION);
    }
       oldBitmap = SelectObject(hdcMem, hBitmap);

       GetObject(hBitmap, sizeof(bitmap), &bitmap);

        int i = (PreOrderStr[num])-'1';//char to int
       BitBlt(hdc, ball_xy[i].bx, ball_xy[i].by,
              bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

       ball_str1="bmp\\";
       SelectObject(hdcMem, oldBitmap);

       }

     DeleteDC(hdcMem);
     EndPaint(hwndDlg, &ps);

    }return TRUE;


    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case IDC_ADD:{
                //GET TEXT.
              HWND hCtrl = GetDlgItem(hwndDlg, IDC_EDIT);
                    int len = GetWindowTextLength( hCtrl );
                if(len > 0){
                char* buf;
                buf = (char*) GlobalAlloc(GPTR, len + 1);
                GetWindowText (hCtrl, buf, len + 1);
                //insert data
                insertData(&anode,atoi(buf));
                PreOrderStr="";
                level=0;
                PreOrder(anode,hwndDlg);

                RedrawWindow(hwndDlg,NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
               //
               // MessageBox    (NULL, buf, "YOU ENTERED:", MB_ICONERROR | MB_OK | MB_TOPMOST);
                //HWND myResult = GetDlgItem(hwndDlg, IDC_RESULT);               //Get Control Handle.
                //SetWindowText(myResult, buf);
                GlobalFree    ((HANDLE)buf);
                }
              return TRUE;
              }

            case IDC_SUB:{
                //GET TEXT.
              HWND hCtrl = GetDlgItem(hwndDlg, IDC_EDIT);
              GetText(hCtrl,hwndDlg);
              return TRUE;
              }
             case IDC_SEARCH:{
                //GET TEXT.
              HWND hCtrl = GetDlgItem(hwndDlg, IDC_EDIT);
              GetText(hCtrl,hwndDlg);
              return TRUE;
              }
    //IDC_EDIT.---------------------------------------------------
            case IDC_EDIT: {
              switch(HIWORD(wParam)) {  //lParam = hCtrl

                case EN_CHANGE:
                 // MessageBox(NULL, "Text was changed"    , "EN_CHANGE"   , MB_OK);
                  return TRUE;

                case EN_UPDATE:
                  //MessageBox(NULL, "Text will be updated", "EN_UPDATE"   , MB_OK);
                  return TRUE;

                case EN_KILLFOCUS:
                  //MessageBox(NULL, "Focus will be gone"  , "EN_KILLFOCUS", MB_OK);
                  return TRUE;

                case EN_SETFOCUS:
                  //MessageBox(NULL, "Will receive focus", "EN_SETFOCUS" , MB_OK);
                  return TRUE;

              }
              return FALSE;
            }
        }
     }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
