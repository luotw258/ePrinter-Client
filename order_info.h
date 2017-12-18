#ifndef __ORDER_INFO_H_
#define __ORDER_INFO_H_


typedef struct order_info {
	int 			fprinterno;
	int 			fnum;
	int 			fdownload;
	int 			fprint;
	int 			fside;
	int 			fcolor;
	int 			fstart;
	int 			fend;
	int 			fcopies;
	int 			fpapersize;
	int 			fsidenum;
	int 			fstyle;
	int 			fmodel;
	int			    fplantime;
	int             fspent;
	int64_t 		fno;
	char			uphone[12];
	char			fuploadtime[20];
	char			fname[256];
	char			furl[256];
} OrderInfo;

#endif // !__ORDER_INFO_H_
