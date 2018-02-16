#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#define PBEGIN 41
FILE *ifp,*ofp,*sizeGetter;
int palettenum;
int colornum;
long int oriSize;
void mainmenu();
void printTitle();
char *targetFileName=(char *)malloc(1000);

void saveFile() {
	fclose(ifp);
	fclose(ofp);
	char *command=(char *)malloc(1000);
	strcpy(command,"rename TEMP.img \"");
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(targetFileName,drive,dir,fname,ext);
	char fname2[_MAX_FNAME];
	strcpy(fname2,fname);
	strcpy(fname2,strcat(fname2,".img"));
	if (_access(fname2,0)==0) {
		char *delCommand=(char *)malloc(1000);
		strcpy(delCommand,"del \"");
		strcpy(delCommand,strcat(delCommand,fname));
		strcpy(delCommand,strcat(delCommand,".img\""));
		//printf("%s\n",delCommand);
		system(delCommand);
	}
	strcpy(command,strcat(command,fname)); //TODO: targetFileName includes path, delete the path
	strcpy(command,strcat(command,".img\""));
	//printf("%s\n",command);
	system(command);
}

void hide() {
	//
	int index;
	printf("请输入要隐藏的调色板编号(1-%d)>",palettenum);
	scanf("%d",&index);
	if (index<1||index>palettenum) {
		printf("调色板编号小于1或者大于%d，请重新输入！\n",palettenum);
		hide();
	}
	fputc(palettenum,ofp); //palette number
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	fputc(colornum,ofp); //color number
	for (int tmp2=1;tmp2<=3;tmp2++) {
		fputc(0,ofp);
		fgetc(ifp);
	}
	for (int tmp31=1;tmp31<=index-1;tmp31++) { //palettes
		for (int tmp32=1;tmp32<=4*(colornum+1);tmp32++) { //colors
			char tmp;
			tmp=fgetc(ifp);
			//if (index!=palettenum||tmp32<=4*colornum||tmp31!=index-1) 
				fputc(tmp,ofp); //离散数学有段时间没搞手生了。。。
		}
	}
	//fputc(colornum,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp); //hided palette
	for (int tmp4=1;tmp4<(colornum+1)*4;tmp4++) {
		char tmp;
		tmp=fgetc(ifp);
		if (tmp4%4==0) fputc(0,ofp);
		else fputc(tmp,ofp);
	}
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//printf("\n%x ",tmp_else);
	//system("cls");
	//printf("文件字节数：%d\n",ftell(ifp));
	printf("隐藏调色板成功，已在原目录生成%s\n",targetFileName);
	fseek(ifp,0,PBEGIN);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void overwrite() {
	int index,index2;
	printf("请输入要复制的调色板编号(1-%d)>",palettenum);
	scanf("%d",&index);
	if (index<1||index>palettenum) {
		printf("调色板编号小于1或者大于%d，请重新输入！\n",palettenum);
		overwrite();
	}
	printf("请输入要覆盖的调色板编号(1-255)>");
	scanf("%d",&index2);
	if (index2<1||index2>255) {
		printf("调色板编号小于1或者大于255，请重新输入！\n");
		overwrite();
	}
	if (index2>palettenum) fputc(index2,ofp);
	else fputc(palettenum,ofp);
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	if (index2>palettenum) { //over than palettes
		fputc(colornum,ofp); //color number
		for (int tmp2=1;tmp2<=3;tmp2++) {
			fputc(0,ofp);
			fgetc(ifp);
		}
		for (int tmp31=1;tmp31<=palettenum;tmp31++) { //palettes
			for (int tmp32=1;tmp32<=4*(colornum+1);tmp32++) { //colors
				char tmp;
				if (tmp31!=palettenum||tmp32<=4*colornum) {
					tmp=fgetc(ifp);
					fputc(tmp,ofp);
				}
			}
		}
		for (int tmp4=1;tmp4<index2-palettenum;tmp4++) { //blank interval
			fputc(colornum,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);
			for (int tmp41=1;tmp41<=colornum;tmp41++) {
				fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0xff,ofp);
			}
		}
		fputc(colornum,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);
		fseek(ifp,40+(index-1)*4*(colornum+1),SEEK_SET); //copied target place
		//printf("\n%d\n",ftell(ifp));
		for (int tmp5=1;tmp5<(colornum+1)*4;tmp5++) {
			char tmp;
			tmp=fgetc(ifp);
			fputc(tmp,ofp);
		}
		fseek(ifp,40+palettenum*4*(colornum+1)-1,SEEK_SET); //how to recover?
		//TODO: NOT SOLVED - no not more than current palettes
	} else { //not over than palettes
		fputc(colornum,ofp); //color number
		for (int tmp2=1;tmp2<=3;tmp2++) {
			fputc(0,ofp);
			fgetc(ifp);
		}
		for (int tmp31=1;tmp31<=index2-1;tmp31++) { //palettes
			for (int tmp32=1;tmp32<=4*(colornum+1);tmp32++) { //colors
				char tmp;
				tmp=fgetc(ifp);
				//if (index!=palettenum||tmp32<=4*colornum||tmp31!=index-1) 
				fputc(tmp,ofp); //离散数学有段时间没搞手生了。。。
			}
		}
		//printf("\n%d\n",ftell(ifp));
		//printf("\n%d\n",40+(index-1)*4*(colornum+1));
		fseek(ifp,40+(index-1)*4*(colornum+1),SEEK_SET);
		//printf("\n%d\n",ftell(ifp));
		for (int tmp4=1;tmp4<(colornum+1)*4;tmp4++) {
			char tmp;
			tmp=fgetc(ifp);
			if (tmp4<(colornum+1)*4-3) fputc(tmp,ofp);
		}
		fseek(ifp,ftell(ofp),SEEK_SET);
	}
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//printf("\n%x ",tmp_else);
	//system("cls");
	//printf("文件字节数：%d\n",ftell(ifp));
	printf("覆盖调色板成功，已在原目录生成%s\n",targetFileName);
	fseek(ifp,0,PBEGIN);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void add() {
	//ADD
	//fseek(ofp,32);
	fputc(palettenum+1,ofp); //palette number
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	fputc(colornum,ofp); //color number
	for (int tmp2=1;tmp2<=3;tmp2++) {
		fputc(0,ofp);
	}
	for (int tmp21=1;tmp21<=3;tmp21++) { //move forward
		fgetc(ifp);
	}
	for (int tmp3=1;tmp3<=palettenum*(colornum+1)*4-4;tmp3++) { //get current palettes
		char tmp;
		tmp=fgetc(ifp);
		fputc(tmp,ofp);
	}
	fputc(colornum,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp); //add an interval
	fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);
	for (int tmp4=1;tmp4<=colornum-1;tmp4++) { //add a new palette
		fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);fputc(255,ofp);
	}
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//printf("\n%x ",tmp_else);
	//system("cls");
	//printf("文件字节数：%d\n",ftell(ifp));
	printf("添加调色板成功，已在原目录生成%s\n",targetFileName);
	fseek(ifp,0,PBEGIN);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void delete1() {
	int index;
	printf("请输入要删除的调色板编号(1-%d)>",palettenum);
	scanf("%d",&index);
	if (index<1||index>palettenum) {
		printf("调色板编号小于1或者大于%d，请重新输入！\n",palettenum);
		delete1();
	}
	fputc(palettenum-1,ofp); //palette number
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	fputc(colornum,ofp); //color number
	for (int tmp2=1;tmp2<=3;tmp2++) {
		fputc(0,ofp);
		fgetc(ifp);
	}
	for (int tmp31=1;tmp31<=index-1;tmp31++) { //palettes
		for (int tmp32=1;tmp32<=4*(colornum+1);tmp32++) { //colors
			char tmp;
			tmp=fgetc(ifp);
			if (index!=palettenum||tmp32<=4*colornum||tmp31!=index-1) fputc(tmp,ofp); //离散数学有段时间没搞手生了。。。
		}
	}
	if (index<palettenum) {
		for (int tmp4=1;tmp4<=4*(colornum+1);tmp4++) { //deleted palette bytes
			fgetc(ifp);
		}
	}
	if (index==palettenum) {
		for (int tmp4=1;tmp4<=4*colornum;tmp4++) { //deleted palette bytes
			fgetc(ifp);
		}
	}
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//printf("\n%x ",tmp_else);
	//system("cls");
	//printf("文件字节数：%d\n",ftell(ifp));
	printf("删除调色板成功，已在原目录生成%s\n",targetFileName);
	fseek(ifp,0,PBEGIN);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void edit(short index) {
	fputc(palettenum,ofp); //palette number
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	fputc(colornum,ofp); //color number
	for (int tmp2=1;tmp2<=3;tmp2++) {
		fputc(0,ofp);
		fgetc(ifp);
	}
	for (int tmp31=1;tmp31<=index-1;tmp31++) { //palettes
		for (int tmp32=1;tmp32<=4*(colornum+1);tmp32++) { //colors
			char tmp;
			tmp=fgetc(ifp);
			fputc(tmp,ofp);
		}
	}
	int pointerColor[4];
	for (int tmp4=1;tmp4<=colornum;tmp4++) {
		printf("当前第%d个调色板的第%d个颜色的RGBA值为：\n",index,tmp4);
		int origialPointerColor[4];
		origialPointerColor[0]=fgetc(ifp);
		origialPointerColor[1]=fgetc(ifp);
		origialPointerColor[2]=fgetc(ifp);
		origialPointerColor[3]=fgetc(ifp);
		printf("%d %d %d %d\n",origialPointerColor[0],origialPointerColor[1],origialPointerColor[2],origialPointerColor[3]);
		printf("修改第%d个颜色，一共%d个颜色\n中间用任意个空格分隔，按上方向键可以自动输入上一次的输入的值>",tmp4,colornum);
		do {
			scanf("%d %d %d %d",&pointerColor[0],&pointerColor[1],&pointerColor[2],&pointerColor[3]);
			if (
				pointerColor[0]<0||pointerColor[0]>255||
				pointerColor[1]<0||pointerColor[1]>255||
				pointerColor[2]<0||pointerColor[2]>255||
				pointerColor[3]<0||pointerColor[3]>255
			) {
				printf("发现有一个或多个值小于0或大于255！请重新输入该项。\n");
				printf("修改第%d个颜色，一共%d个颜色\n中间用任意个空格分隔，按上方向键可以自动输入上一次的输入的值>",tmp4,colornum);
			}
		} while (
			pointerColor[0]<0||pointerColor[0]>255||
			pointerColor[1]<0||pointerColor[1]>255||
			pointerColor[2]<0||pointerColor[2]>255||
			pointerColor[3]<0||pointerColor[3]>255
		);
		fputc(pointerColor[0],ofp);
		fputc(pointerColor[1],ofp);
		fputc(pointerColor[2],ofp);
		fputc(pointerColor[3],ofp);
		//interval!
	}
	if (index!=palettenum) {
		fgetc(ifp);fgetc(ifp);fgetc(ifp);fgetc(ifp);
		fputc(colornum,ofp);fputc(0,ofp);fputc(0,ofp);fputc(0,ofp);
	}
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//system("cls");
	//TODO
	printf("修改第%d个调色板的详细数据成功，已在原目录生成%s\n",index,targetFileName);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void editForm() {
	//
	short index=-1;
	printf("请输入要修改的调色板编号(1-%d)>",palettenum);
	scanf("%hd",&index);
	if (index<1||index>palettenum) {
		printf("调色板编号小于1或者大于%d，请重新输入！\n",palettenum);
		editForm();
	}
	edit(index);
}

void printTitle() {
	printf("DNF Version 6 IMG 调色板编辑器 V0.02 Beta BY 鄙人\n");
	printf("通用ID：wujiuqier 首发EXNPK论坛 DNF美服吧官方交流群：537136144\n");
	printf("本程序适用于BSD开源许可协议 程序用C语言编写\n");
	printf("目前暂时只支持一次运行完成一次操作，切换操作功能需要重构代码，望见谅。\n");
	printf("特别鸣谢班上的Java Web老师帮助鄙人修复了多个BUG\n");
	printf("转载请注明出处：wujiuqier，EXNPK论坛\n\n");
}

void mainmenu() {
	printf("\n1. 添加一种空白调色板\n");
	printf("2. 删除一种调色板\n");
	printf("3. 修改一种调色板的详细数据\n");
	printf("4. 隐藏一种调色板\n");
	printf("5. 复制并覆盖另一种调色板\n");
	printf("其他. 退出\n请输入你要选择的操作，然后回车进入>");
	int command=0;
	scanf("%d",&command);
	if (command==1) {
		//add
		add();
	}
	if (command==2) {
		//remove
		delete1();
	}
	if (command==3) {
		//edit
		editForm();
	}
	if (command==4) {
		//hide
		hide();
	}
	if (command==5) {
		//overwrite
		overwrite();
	}
	else {
		//exit
		fclose(ifp);
		fclose(ofp);
		system("del TEMP.img");
		exit(0);
	}
}

void convertv6() {
	fputc(palettenum,ofp); //palette number
	for (int tmp1=1;tmp1<=3;tmp1++) {
		fputc(0,ofp);
	}
	fputc(colornum,ofp);
	char tmp_else;
	int i=0;
	while (!feof(ifp)) { //other bytes below
		i++;
		tmp_else=fgetc(ifp);
		if (!feof(ifp)) fputc(tmp_else,ofp);
		//feof(ifp)
	}
	fclose(ofp);
	//printf("\n%x ",tmp_else);
	//system("cls");
	//printf("文件字节数：%d\n",ftell(ifp));
	printf("转换为Ver6成功，已在原目录生成%s\n",targetFileName);
	fseek(ifp,0,PBEGIN);
	//printTitle();
	//mainmenu();
	saveFile();
	printf("按任意键退出 . . .");
	getch();
	exit(0);
}

void mainmenuv4() {
	printf("\n1. 转换IMG版本到Ver6\n");
	printf("其他. 退出\n请输入你要选择的操作，然后回车进入>");
	int command=0;
	scanf("%d",&command);
	if (command==1) {
		//converttover6
		convertv6();
	}
	else {
		//exit
		fclose(ifp);
		fclose(ofp);
		system("del TEMP.img");
		exit(0);
	}
}

void main(int argc, char* argv[]) {
	//TODO 
	int i;
	//system("cls");
	printTitle();
	if (argc==1) {
		printf("该程序必须使用打开方式或者命令行加入文件路径参数才能使用！\n");
		printf("建议：右键拖动要修改的IMG到这个程序文件的图标，选择打开方式，即可打开。\n");
		printf("按任意键退出 . . .");
		getch();
		exit(-1);
	}
	printf("选择的文件是：%s\n", argv[1]);
	if ((ifp=fopen(argv[1],"rb"))==NULL) {
		printf("打开文件失败！按任意键退出。\n");
		getch();
		exit(-1);
	} 
	//fseek(ifp,0,SEEK_END);
	//oriSize=ftell(ifp);
	if ((sizeGetter=fopen(argv[1],"rb"))==NULL) {
		printf("打开文件失败！按任意键退出。\n");
		getch();
		exit(-1);
	} 
	fseek(sizeGetter,0,SEEK_END);
	oriSize=ftell(sizeGetter);
	printf("文件字节数：%ld\n",oriSize);
	strcpy(targetFileName,strcat(argv[1],".generated.img"));
	ofp=fopen("TEMP.img","wb");
	for (i=0;i<0x18;i++) {
		char tmp;
		tmp=fgetc(ifp);
		fputc(tmp,ofp);
	}
	int ver=fgetc(ifp);
	if (ver==6) fputc(ver,ofp);
	if (ver==4) fputc(0x06,ofp);
	printf("IMG 版本            ：%d\n",ver);
	if (ver!=6&&ver!=4) {
		printf("住手！这根本不是ver4或者ver6的IMG！按任意键退出。\n");
		getch();
		fclose(ofp);
		fclose(ifp);
		system("del TEMP.img");
		exit(-1);
	}
	for (i=0;i<0x07;i++) {
		char tmp;
		tmp=fgetc(ifp);
		fputc(tmp,ofp);
	}
	if (ver==6) {
		palettenum=fgetc(ifp);
		printf("调色板数量          ：%d\n",palettenum);
		for (i=0;i<0x03;i++) fgetc(ifp);
		colornum=fgetc(ifp);
		printf("每种调色板的颜色数量：%d\n",colornum);
		mainmenu();
	}
	if (ver==4) {
		palettenum=1;
		printf("调色板数量          ：%d\n",palettenum);
		colornum=fgetc(ifp);
		printf("每种调色板的颜色数量：%d\n",colornum);
		mainmenuv4();
	}
}
