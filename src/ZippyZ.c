/*
 * Zippy Z - Native ZIP icon chooser for Windows 11
 * Version 2.0.1, 2026-11-11
 * Copyright (c) 2026 Mark McEnarney
 * MIT Licence - see LICENSE
 *
 * Standalone Win32 GUI (no PowerShell, CMD, .NET, runtime compilation or network).
 * Modifies only two per-user ZIP icon defaults; it does NOT change file associations.
 * This file intentionally uses minimal Windows API declarations to permit cross-builds
 * without a Windows SDK. See BUILD.md for repeatable Windows SDK build alternative.
 */

typedef unsigned short wchar_t;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef unsigned int UINT;
typedef unsigned long long UINT_PTR;
typedef unsigned long long WPARAM;
typedef long long LPARAM;
typedef long long LRESULT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef int BOOL;
typedef long HRESULT;
typedef long LONG;
typedef void* HANDLE;
typedef HANDLE HWND;
typedef HANDLE HINSTANCE;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HFONT;
typedef HANDLE HKEY;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
typedef LRESULT (__stdcall *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct { LONG x, y; } POINT;
typedef struct {
    HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam;
    DWORD time; POINT pt; DWORD lPrivate;
} MSG;
typedef struct {
    UINT cbSize, style; WNDPROC lpfnWndProc;
    int cbClsExtra, cbWndExtra; HINSTANCE hInstance; HICON hIcon;
    HCURSOR hCursor; HBRUSH hbrBackground; LPCWSTR lpszMenuName;
    LPCWSTR lpszClassName; HICON hIconSm;
} WNDCLASSEXW;

#define IMPORT __declspec(dllimport)
IMPORT HINSTANCE __stdcall GetModuleHandleW(LPCWSTR);
IMPORT DWORD __stdcall GetModuleFileNameW(HINSTANCE, LPWSTR, DWORD);
IMPORT DWORD __stdcall GetFileAttributesW(LPCWSTR);
IMPORT BOOL __stdcall CopyFileW(LPCWSTR, LPCWSTR, BOOL);
IMPORT BOOL __stdcall CreateDirectoryW(LPCWSTR, void*);
IMPORT void __stdcall ExitProcess(UINT);
IMPORT unsigned short __stdcall RegisterClassExW(const WNDCLASSEXW*);
IMPORT HWND __stdcall CreateWindowExW(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HANDLE,HINSTANCE,void*);
IMPORT LRESULT __stdcall DefWindowProcW(HWND,UINT,WPARAM,LPARAM);
IMPORT BOOL __stdcall ShowWindow(HWND,int);
IMPORT BOOL __stdcall UpdateWindow(HWND);
IMPORT int __stdcall GetMessageW(MSG*,HWND,UINT,UINT);
IMPORT BOOL __stdcall TranslateMessage(const MSG*);
IMPORT LRESULT __stdcall DispatchMessageW(const MSG*);
IMPORT void __stdcall PostQuitMessage(int);
IMPORT LRESULT __stdcall SendMessageW(HWND,UINT,WPARAM,LPARAM);
IMPORT BOOL __stdcall SetWindowTextW(HWND,LPCWSTR);
IMPORT HANDLE __stdcall LoadImageW(HINSTANCE,LPCWSTR,UINT,int,int,UINT);
IMPORT BOOL __stdcall DestroyIcon(HICON);
IMPORT HCURSOR __stdcall LoadCursorW(HINSTANCE,LPCWSTR);
IMPORT int __stdcall MessageBoxW(HWND,LPCWSTR,LPCWSTR,UINT);
IMPORT HANDLE __stdcall GetStockObject(int);
IMPORT LONG __stdcall RegOpenKeyExW(HKEY,LPCWSTR,DWORD,DWORD,HKEY*);
IMPORT LONG __stdcall RegCreateKeyExW(HKEY,LPCWSTR,DWORD,LPWSTR,DWORD,DWORD,void*,HKEY*,DWORD*);
IMPORT LONG __stdcall RegQueryValueExW(HKEY,LPCWSTR,DWORD*,DWORD*,BYTE*,DWORD*);
IMPORT LONG __stdcall RegSetValueExW(HKEY,LPCWSTR,DWORD,DWORD,const BYTE*,DWORD);
IMPORT LONG __stdcall RegDeleteValueW(HKEY,LPCWSTR);
IMPORT LONG __stdcall RegCloseKey(HKEY);
IMPORT HRESULT __stdcall SHGetFolderPathW(HWND,int,HANDLE,DWORD,LPWSTR);
IMPORT void __stdcall SHChangeNotify(LONG,UINT,const void*,const void*);

#define HKCU ((HKEY)(long long)(long)0x80000001UL)
#define KEY_READ 0x20019u
#define KEY_WRITE 0x20006u
#define REG_SZ 1u
#define REG_BINARY 3u
#define REG_DWORD 4u
#define WS_OVERLAPPEDWINDOW 0x00CF0000u
#define WS_CHILD 0x40000000u
#define WS_VISIBLE 0x10000000u
#define WS_BORDER 0x00800000u
#define WS_VSCROLL 0x00200000u
#define WS_EX_CLIENTEDGE 0x00000200u
#define LBS_NOTIFY 0x0001u
#define LBS_NOINTEGRALHEIGHT 0x0100u
#define SS_ICON 3u
#define WM_DESTROY 2u
#define WM_COMMAND 0x0111u
#define WM_SETFONT 0x0030u
#define STM_SETIMAGE 0x0172u
#define LB_ADDSTRING 0x0180u
#define LB_SETCURSEL 0x0186u
#define LB_GETCURSEL 0x0188u
#define LBN_SELCHANGE 1u
#define BN_CLICKED 0u
#define IMAGE_ICON 1u
#define LR_LOADFROMFILE 0x0010u
#define MB_ICONERROR 0x0010u
#define MB_ICONINFORMATION 0x0040u
#define MB_ICONQUESTION 0x0020u
#define MB_YESNO 0x0004u
#define IDYES 6
#define CSIDL_LOCAL_APPDATA 0x001c
#define SHCNE_ASSOCCHANGED 0x08000000
#define SW_SHOWNORMAL 1
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFFu
#define IDC_LIST 100
#define IDC_APPLY 101
#define IDC_RESTORE 102
#define IDC_ABOUT 103
#define NO_ERROR 0
#define ERROR_FILE_NOT_FOUND 2
#define MAX_PATH_BUF 2048
#define MAX_VALUE_BYTES 8192

static const wchar_t *kClass = L"ZippyZNativeChooser";
static const wchar_t *kRegClass[2] = {
    L"Software\\Classes\\.zip\\DefaultIcon",
    L"Software\\Classes\\CompressedFolder\\DefaultIcon"
};
static const wchar_t *kBackupRoot = L"Software\\ZippyZNative\\Backup";

struct Choice { const wchar_t *label; const wchar_t *file; const wchar_t *about; };
static const struct Choice kChoices[9] = {
    {L"Chestnut Zippy - brown folder", L"01-Chestnut-Zippy.ico", L"Warm brown folder, clear blue Z and silver zipper."},
    {L"Golden Seal - yellow folder", L"02-Golden-Seal.ico", L"Bright gold folder with a compact Z emblem."},
    {L"Midnight Folder", L"03-Midnight-Folder.ico", L"Dark blue folder with the Zippy Z identity."},
    {L"Copper Archive", L"04-Copper-Archive.ico", L"Copper-coloured archive folder."},
    {L"Zipper Document", L"05-Zipper-Document.ico", L"Document-style ZIP icon with a zipper motif."},
    {L"Electric Z", L"06-Electric-Z.ico", L"Strong blue Z with an archive-style detail."},
    {L"Charcoal Wallet", L"07-Charcoal-Wallet.ico", L"A dark, restrained ZIP file design."},
    {L"Teal Archive", L"08-Teal-Archive.ico", L"Teal-coloured archive icon."},
    {L"Original Zippy Z", L"09-Earlier-Zippy-Z.ico", L"The earlier blue Z and silver zipper design."}
};

struct SavedValue { DWORD exists, type, size; BYTE data[MAX_VALUE_BYTES]; };
static HWND gMain, gList, gImage, gHeading, gDescription, gStatus;
static HICON gShownIcon, gAppIconLarge, gAppIconSmall;
static wchar_t gExeDir[MAX_PATH_BUF];
static HINSTANCE gInst;

static unsigned str_len(const wchar_t *s) { unsigned n=0; while(s && s[n])n++; return n; }
static BOOL append(wchar_t *dest, unsigned cap, const wchar_t *src) {
    unsigned at=str_len(dest), i=0; if(!src) return 0;
    while(src[i]) {if(at+i+1>=cap) return 0;dest[at+i]=src[i];i++;}
    dest[at+i]=0;return 1;
}
static BOOL copystr(wchar_t *dest,unsigned cap,const wchar_t *src) {
    if(!cap) return 0;dest[0]=0;return append(dest,cap,src);
}
static BOOL path_join(wchar_t *out,unsigned cap,const wchar_t *a,const wchar_t *b) {
    if(!copystr(out,cap,a))return 0;
    if(str_len(out) && out[str_len(out)-1]!=L'\\') if(!append(out,cap,L"\\"))return 0;
    return append(out,cap,b);
}
static BOOL path_icon(unsigned idx,wchar_t *dest,unsigned cap) {
    wchar_t dir[MAX_PATH_BUF];
    return (idx<9 && path_join(dir,MAX_PATH_BUF,gExeDir,L"Icons") && path_join(dest,cap,dir,kChoices[idx].file));
}
static void failure(LPCWSTR msg) { MessageBoxW(gMain,msg,L"Zippy Z - operation not completed",MB_ICONERROR); }
static BOOL create_key(LPCWSTR name,HKEY *out) {
    DWORD disposition=0;
    return RegCreateKeyExW(HKCU,name,0,0,0,KEY_READ|KEY_WRITE,0,out,&disposition)==0;
}
static BOOL get_dword(HKEY h,LPCWSTR name,DWORD *out) {
    DWORD t=0,n=sizeof(DWORD);
    return RegQueryValueExW(h,name,0,&t,(BYTE*)out,&n)==0 && t==REG_DWORD && n==sizeof(DWORD);
}
static BOOL put_dword(HKEY h,LPCWSTR name,DWORD value) {
    return RegSetValueExW(h,name,0,REG_DWORD,(const BYTE*)&value,sizeof(DWORD))==0;
}
static BOOL read_icon_setting(LPCWSTR keyName,struct SavedValue *v) {
    HKEY h=0;LONG r;DWORD count=MAX_VALUE_BYTES;
    v->exists=0;v->size=0;v->type=REG_SZ;
    r=RegOpenKeyExW(HKCU,keyName,0,KEY_READ,&h);
    if(r==ERROR_FILE_NOT_FOUND) return 1;
    if(r!=0) return 0;
    r=RegQueryValueExW(h,0,0,&v->type,v->data,&count);
    RegCloseKey(h);
    if(r==ERROR_FILE_NOT_FOUND) return 1;
    if(r!=0) return 0;
    v->exists=1;v->size=count;
    return 1;
}
static BOOL write_icon_setting(LPCWSTR keyName,const struct SavedValue *v) {
    HKEY h=0;LONG r;
    if(v->exists) {
        if(!create_key(keyName,&h)) return 0;
        r=RegSetValueExW(h,0,0,v->type,v->data,v->size);
        RegCloseKey(h);
        return r==0;
    }
    r=RegOpenKeyExW(HKCU,keyName,0,KEY_READ|KEY_WRITE,&h);
    if(r==ERROR_FILE_NOT_FOUND) return 1;
    if(r!=0) return 0;
    r=RegDeleteValueW(h,0);
    RegCloseKey(h);
    return r==0 || r==ERROR_FILE_NOT_FOUND;
}
static BOOL write_new_icon(LPCWSTR keyName,LPCWSTR iconLocation) {
    HKEY h=0;LONG r;
    if(!create_key(keyName,&h)) return 0;
    r=RegSetValueExW(h,0,0,REG_SZ,(const BYTE*)iconLocation,(str_len(iconLocation)+1)*sizeof(wchar_t));
    RegCloseKey(h);
    return r==0;
}
static BOOL save_snapshot_if_missing(void) {
    HKEY h=0;DWORD complete=0;
    if(!create_key(kBackupRoot,&h))return 0;
    if(get_dword(h,L"Complete",&complete) && complete==1) {
        RegCloseKey(h);return 1;
    }
    /* A failed earlier snapshot can be safely replaced until Complete == 1. */
    for(unsigned i=0;i<2;i++) {
        struct SavedValue v;
        LPCWSTR nameExists=i==0?L"ZipExists":L"ClassExists";
        LPCWSTR nameType=i==0?L"ZipType":L"ClassType";
        LPCWSTR nameData=i==0?L"ZipData":L"ClassData";
        if(!read_icon_setting(kRegClass[i],&v) ||
           !put_dword(h,nameExists,v.exists) ||
           !put_dword(h,nameType,v.type) ||
           RegSetValueExW(h,nameData,0,REG_BINARY,v.data,v.size)!=0) {
            RegCloseKey(h);return 0;
        }
    }
    if(!put_dword(h,L"Complete",1)) {RegCloseKey(h);return 0;}
    RegCloseKey(h);return 1;
}
static BOOL read_saved_snapshot(struct SavedValue *a,struct SavedValue *b) {
    HKEY h=0;DWORD complete=0;
    if(RegOpenKeyExW(HKCU,kBackupRoot,0,KEY_READ,&h)!=0)return 0;
    if(!get_dword(h,L"Complete",&complete)||complete!=1){RegCloseKey(h);return 0;}
    for(unsigned i=0;i<2;i++) {
        struct SavedValue *v=i==0?a:b;
        LPCWSTR nameExists=i==0?L"ZipExists":L"ClassExists";
        LPCWSTR nameType=i==0?L"ZipType":L"ClassType";
        LPCWSTR nameData=i==0?L"ZipData":L"ClassData";
        DWORD t=0,n=MAX_VALUE_BYTES;
        if(!get_dword(h,nameExists,&v->exists) ||
           !get_dword(h,nameType,&v->type) ||
           RegQueryValueExW(h,nameData,0,&t,v->data,&n)!=0 || t!=REG_BINARY || n>MAX_VALUE_BYTES) {
            RegCloseKey(h);return 0;
        }
        v->size=n;
    }
    RegCloseKey(h);return 1;
}
static void refresh_shell(void) { SHChangeNotify(SHCNE_ASSOCCHANGED,0,0,0); }
static BOOL ensure_storage(wchar_t *iconsDir,unsigned cap) {
    wchar_t base[MAX_PATH_BUF];
    if(SHGetFolderPathW(0,CSIDL_LOCAL_APPDATA,0,0,base)!=0)return 0;
    if(!path_join(iconsDir,cap,base,L"ZippyZNative"))return 0;
    if(!CreateDirectoryW(iconsDir,0) && GetFileAttributesW(iconsDir)==INVALID_FILE_ATTRIBUTES)return 0;
    if(!append(iconsDir,cap,L"\\Icons"))return 0;
    if(!CreateDirectoryW(iconsDir,0) && GetFileAttributesW(iconsDir)==INVALID_FILE_ATTRIBUTES)return 0;
    return 1;
}
static void apply_selected(void) {
    int idx=(int)SendMessageW(gList,LB_GETCURSEL,0,0);
    wchar_t src[MAX_PATH_BUF], iconsDir[MAX_PATH_BUF], dst[MAX_PATH_BUF], iconValue[MAX_PATH_BUF];
    struct SavedValue oldZip,oldClass;
    if(idx<0 || idx>=9) {failure(L"Please select an icon first.");return;}
    if(!path_icon((unsigned)idx,src,MAX_PATH_BUF) || GetFileAttributesW(src)==INVALID_FILE_ATTRIBUTES) {
        failure(L"The selected .ico file is missing from the Icons folder. Please extract the complete ZIP package first.");return;
    }
    if(!ensure_storage(iconsDir,MAX_PATH_BUF) ||
       !path_join(dst,MAX_PATH_BUF,iconsDir,kChoices[idx].file) ||
       !CopyFileW(src,dst,0) ||
       !copystr(iconValue,MAX_PATH_BUF,dst) ||
       !append(iconValue,MAX_PATH_BUF,L",0")) {
        failure(L"Windows could not copy the icon to your local application data folder. The ZIP icon settings have not been changed.");return;
    }
    if(!read_icon_setting(kRegClass[0],&oldZip) || !read_icon_setting(kRegClass[1],&oldClass)) {
        failure(L"Windows could not read the existing ZIP icon settings. No change was made.");return;
    }
    if(!save_snapshot_if_missing()) {
        failure(L"Windows could not preserve the previous ZIP icon settings. No change was made.");return;
    }
    if(!write_new_icon(kRegClass[0],iconValue) || !write_new_icon(kRegClass[1],iconValue)) {
        write_icon_setting(kRegClass[0],&oldZip);
        write_icon_setting(kRegClass[1],&oldClass);
        refresh_shell();
        failure(L"Windows refused the ZIP icon change. An attempt was made to roll back the current settings.");return;
    }
    refresh_shell();
    SetWindowTextW(gStatus,L"Applied. If Explorer still shows an old icon, sign out and back in.");
    MessageBoxW(gMain,L"The selected ZIP icon was applied for your Windows account. Your ZIP files and their default opening app are unchanged.\n\nExplorer may need a sign-out and sign-in to refresh its icons.",L"Zippy Z",MB_ICONINFORMATION);
}
static void restore_saved(void) {
    struct SavedValue v0,v1,now0,now1;
    if(!read_saved_snapshot(&v0,&v1)) {
        failure(L"There is no complete Zippy Z Native backup to restore. This version does not erase other programmes' ZIP icon settings.");return;
    }
    if(MessageBoxW(gMain,L"Restore the two ZIP icon values saved before Zippy Z Native first changed them?\n\nThis will not modify how ZIP files open.",L"Restore previous ZIP icons",MB_YESNO|MB_ICONQUESTION)!=IDYES) return;
    if(!read_icon_setting(kRegClass[0],&now0) || !read_icon_setting(kRegClass[1],&now1)) {
        failure(L"Unable to read the current icon settings. No change was made.");return;
    }
    if(!write_icon_setting(kRegClass[0],&v0) || !write_icon_setting(kRegClass[1],&v1)) {
        write_icon_setting(kRegClass[0],&now0);
        write_icon_setting(kRegClass[1],&now1);
        failure(L"Restore failed. An attempt was made to reinstate the current settings.");return;
    }
    refresh_shell();
    SetWindowTextW(gStatus,L"Previous icon settings restored. You may need to sign out and back in.");
    MessageBoxW(gMain,L"Your ZIP icon settings were restored to their state before the first use of Zippy Z Native.\n\nIf an earlier Zippy Z version had already changed the icons, the restored state may include that earlier customisation.",L"Zippy Z",MB_ICONINFORMATION);
}
static void update_preview(void) {
    int idx=(int)SendMessageW(gList,LB_GETCURSEL,0,0);
    if(idx<0||idx>=9)return;
    SetWindowTextW(gHeading,kChoices[idx].label);
    SetWindowTextW(gDescription,kChoices[idx].about);
    wchar_t src[MAX_PATH_BUF];
    HICON icon=0;
    if(path_icon((unsigned)idx,src,MAX_PATH_BUF))
        icon=(HICON)LoadImageW(0,src,IMAGE_ICON,128,128,LR_LOADFROMFILE);
    SendMessageW(gImage,STM_SETIMAGE,IMAGE_ICON,(LPARAM)icon);
    if(gShownIcon)DestroyIcon(gShownIcon);
    gShownIcon=icon;
    SetWindowTextW(gStatus,icon?L"Select an icon, then click Apply.":L"Preview unavailable: one or more icons are missing.");
}
static HWND create_control(LPCWSTR klass,LPCWSTR text,DWORD style,DWORD ex,int x,int y,int w,int h,UINT id) {
    HWND obj=CreateWindowExW(ex,klass,text,style|WS_CHILD|WS_VISIBLE,x,y,w,h,gMain,(HANDLE)(UINT_PTR)id,gInst,0);
    if(obj)SendMessageW(obj,WM_SETFONT,(WPARAM)GetStockObject(17),1);
    return obj;
}
static LRESULT __stdcall window_proc(HWND hwnd,UINT message,WPARAM wp,LPARAM lp) {
    if(message==WM_COMMAND) {
        unsigned id=(unsigned)(wp&0xffffu);
        unsigned notification=(unsigned)((wp>>16)&0xffffu);
        if(id==IDC_LIST && notification==LBN_SELCHANGE)update_preview();
        if(id==IDC_APPLY && notification==BN_CLICKED)apply_selected();
        if(id==IDC_RESTORE && notification==BN_CLICKED)restore_saved();
        if(id==IDC_ABOUT && notification==BN_CLICKED)
            MessageBoxW(gMain,
                L"Zippy Z - ZIP Icon Chooser\nVersion 2.0.1 (Windows x64)\n\n"
                L"Created by Mark McEnarney\nCopyright (c) 2026 Mark McEnarney\n\n"
                L"Released under the MIT Licence.\nSource code and icon artwork are included with the GitHub project.\n\n"
                L"No network access, administrator rights, background processes, or telemetry are required.",
                L"About Zippy Z",MB_ICONINFORMATION);
        return 0;
    }
    if(message==WM_DESTROY) {
        if(gShownIcon)DestroyIcon(gShownIcon);
        PostQuitMessage(0);return 0;
    }
    return DefWindowProcW(hwnd,message,wp,lp);
}
static void show_window(HINSTANCE instance) {
    WNDCLASSEXW wc; MSG msg;
    wchar_t brandIcon[MAX_PATH_BUF];
    if(path_icon(0,brandIcon,MAX_PATH_BUF)) {
        gAppIconLarge=(HICON)LoadImageW(0,brandIcon,IMAGE_ICON,32,32,LR_LOADFROMFILE);
        gAppIconSmall=(HICON)LoadImageW(0,brandIcon,IMAGE_ICON,16,16,LR_LOADFROMFILE);
    }
    wc.cbSize=sizeof(wc);wc.style=0;wc.lpfnWndProc=window_proc;
    wc.cbClsExtra=0;wc.cbWndExtra=0;wc.hInstance=instance;wc.hIcon=gAppIconLarge;
    wc.hCursor=LoadCursorW(0,(LPCWSTR)(unsigned long long)32512);
    wc.hbrBackground=(HBRUSH)(unsigned long long)6;
    wc.lpszMenuName=0;wc.lpszClassName=kClass;wc.hIconSm=gAppIconSmall;
    if(!RegisterClassExW(&wc)){MessageBoxW(0,L"Could not register the Zippy Z window.",L"Zippy Z",MB_ICONERROR);return;}
    gMain=CreateWindowExW(0,kClass,L"Zippy Z - by Mark McEnarney",WS_OVERLAPPEDWINDOW,
                          180,130,835,600,0,0,instance,0);
    if(!gMain){MessageBoxW(0,L"Could not create the Zippy Z window.",L"Zippy Z",MB_ICONERROR);return;}
    create_control(L"STATIC",L"Choose how ZIP files look in Windows 11",0,0,26,24,625,35,0);
    create_control(L"BUTTON",L"About Zippy Z",0,0,665,22,128,34,IDC_ABOUT);
    create_control(L"STATIC",L"Nine ZIP icon designs",0,0,26,75,350,25,0);
    gList=create_control(L"LISTBOX",L"",LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|WS_VSCROLL,WS_EX_CLIENTEDGE,26,105,348,325,IDC_LIST);
    for(unsigned i=0;i<9;i++)SendMessageW(gList,LB_ADDSTRING,0,(LPARAM)kChoices[i].label);
    create_control(L"STATIC",L"Preview",0,0,420,75,330,24,0);
    gImage=create_control(L"STATIC",L"",SS_ICON,0,490,119,160,150,0);
    gHeading=create_control(L"STATIC",L"",0,0,420,291,355,30,0);
    gDescription=create_control(L"STATIC",L"",0,0,420,326,355,66,0);
    create_control(L"BUTTON",L"Apply selected icon",0,0,420,410,180,40,IDC_APPLY);
    create_control(L"BUTTON",L"Restore previous icon",0,0,609,410,184,40,IDC_RESTORE);
    create_control(L"STATIC",L"Changes only the .zip icon for your account. No administrator rights required.",0,0,26,465,770,24,0);
    gStatus=create_control(L"STATIC",L"",0,0,26,493,775,24,0);
    create_control(L"STATIC",L"Zippy Z (c) 2026 Mark McEnarney  |  MIT Licence  |  No administrator rights",0,0,26,527,775,22,0);
    SendMessageW(gList,LB_SETCURSEL,0,0);
    update_preview();
    ShowWindow(gMain,SW_SHOWNORMAL);UpdateWindow(gMain);
    while(GetMessageW(&msg,0,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}
    if(gAppIconLarge)DestroyIcon(gAppIconLarge);
    if(gAppIconSmall)DestroyIcon(gAppIconSmall);
}
void mainCRTStartup(void) {
    gInst=GetModuleHandleW(0);
    DWORD n=GetModuleFileNameW(gInst,gExeDir,MAX_PATH_BUF);
    if(n==0 || n>=MAX_PATH_BUF){MessageBoxW(0,L"Cannot determine the programme's folder.",L"Zippy Z",MB_ICONERROR);ExitProcess(2);}
    while(n>0 && gExeDir[n-1]!=L'\\')n--;
    if(n==0){MessageBoxW(0,L"Cannot locate the bundled Icons folder.",L"Zippy Z",MB_ICONERROR);ExitProcess(2);}
    gExeDir[n-1]=0;
    show_window(gInst);
    ExitProcess(0);
}
