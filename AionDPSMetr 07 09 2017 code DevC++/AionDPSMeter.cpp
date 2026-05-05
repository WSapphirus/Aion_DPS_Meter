#include <windows.h>
#include <fstream>//file
#include <cstring>//strstr
#include <locale> //isalpha
#include <vector>//vector
#include <tchar.h>
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
using namespace std;
char myname[] = "Вы ";
TCHAR buffer[MAX_PATH];

/*--------------------------------------функции отхила и залива маны-------------------------------------------------------*/
class Heal{
      char name[128];
      unsigned int heali;
      unsigned int mana;
      public:
           Heal() {strcpy(name,""); heali = 0; mana = 0;}
           void  puth (unsigned int h) {heali = heali + h;}
           void  putm (unsigned int m) {mana = mana + m;}
           void  putn (char *n) {strcpy (name, n);}
           bool IgnoreHeal();
           bool operator==(Heal op2);
           char names(){return *name;}
           unsigned int hp(){return heali;}
           unsigned int mp(){return mana;}
           int len(){ int i; i=strlen(name); return i;}
           friend ostream &operator << (ostream &stream, Heal obj);
};

bool Heal::IgnoreHeal()//игнорируемые мобы
{
	char ignore[512];
	ifstream init;
	init.open("monsters.txt"); 
	while (init)
		{
			init.getline(ignore,510);
			if(strstr(ignore,name)){return true;}
		}
	init.close();
return false;
}

bool Heal::operator==(Heal op2)
{
        if(strcmp(name,op2.name))
        return false;
        else
return true;
}

ostream &operator <<(ostream &stream, Heal obj)
{
        stream << obj.name << endl;
        stream << "HP:";
        stream << obj.heali << endl;
        stream << "MP:";
        stream << obj.mana << endl;
return stream;       
}

unsigned int Damag(char *str) //Нахождение урона(отхила/маны) в символьном массиве, преобразование типа char в int
{
    int keepers;
    int intel[13]={0};
    int i=0;
    int a=1;
    unsigned int namber=0;
    
	while(!isalpha(*str)) //возращает 0 если это символ, проверить логику функции!
    {
                         keepers = *str;
                         if (keepers!=32 & keepers!=-96)
                         {
                         	if(keepers==58) break;
                                         intel[i]=(keepers-48);
                                         i++;
                         }
                         str++;
    }
    
	i--;
    
	for( ; i >=0; i--)
    {
         namber = namber + (intel[i] * a);
         a = a * 10;
    }
return namber;                                              
}

Heal NameHeal(char *str)
{
    Heal Ambrosia;
    char Name1[128]="";
    char name[128]="";
    int i=0;
    bool b=1;
    
	str = (strstr (str, " : ")+3);
   
    while(b)
    {
            while(!isspace(*str))
            {
                       name[i]=*str;
                       str++;
                       i++;
            }
           if(i>0)
           {
                    if(strstr(name,"использует")||strstr(name,"восстанавливает"))
                    {
                                       b=0;
                    }
                    else
                    {
                              strncat(Name1,name,i);
                              i=1;
                              if(strstr(name,":")) b=0;
                    }   
           }         
    str++;
    strcpy(name," ");
    }
    
	Ambrosia.putn(Name1);
return Ambrosia; 
}

Heal seachHeal(char *str)
{
    Heal Ambrosia;
    
	if(strstr(str, "восстанавливаете"))//хилят меня
    {
       if(strstr(str,"использует:"))
          {
             Ambrosia=NameHeal(str);      
             if(strstr(str,"HP."))
               {
                str=(strstr(str,"восстанавливаете")+17);
                Ambrosia.puth(Damag(str));                
               }
             if(strstr(str,"MP."))       
               {
                str=(strstr(str,"восстанавливаете")+17);
                Ambrosia.putm(Damag(str));                                          
               }
          }
    }
	 
    if(strstr(str,"восстанавливает "))//кто-то хилит кого-то
       {
         if(strstr(str,"использует:"))
         {
            Ambrosia=NameHeal(str);      
            if(strstr(str,"HP."))
             {
             str=(strstr(str,"восстанавливает")+16);
             Ambrosia.puth(Damag(str));               
             }
             if(strstr(str,"MP."))       
             {
             str=(strstr(str,"восстанавливает")+16);
             Ambrosia.putm(Damag(str));                                          
             }
          }
       }  
       
    if(strstr(str,"восстанавливается"))//кто-то хилит кого-то
       {
         if(strstr(str,"использует:"))
         {
            Ambrosia=NameHeal(str);      
            if(strstr(str,"HP."))
             {
             str=(strstr(str,"восстанавливается")+16);
             Ambrosia.puth(Damag(str));               
             }
             if(strstr(str,"MP."))       
             {
             str=(strstr(str,"восстанавливается")+18);
             Ambrosia.putm(Damag(str));                                          
             }
          }
       }
       
return Ambrosia;              
}
/*--------------------------------------функции подсчет урона---------------------------------------------------------*/
class DPS{
      char name[128];
      unsigned int damages, Class;
      double skill, crit;
      public:
           DPS() {strcpy(name,""); Class = 0; damages = 0; crit = 0; skill = 0;}
           void  putd (unsigned int d) {damages = damages + d;}
           void  putn (char *n) {strcpy (name, n);}
           void  putskill (double d) {skill = skill + d;}
           void  putcrit (double d) {crit = crit + d;}
           void Classes(char *str);
           void Clocker(char *str);
           bool operator==(DPS op2);
           bool Dota();
           bool Ignore();
           bool MyName(){if(strcmp(::myname,name))return false;return true;}
           bool TryClass(){if(Class==0)return true; else return false;}
           bool GSClass(){if(Class==12)return true; else return false;}
           bool EUClass(){if(Class==13)return true; else return false;}
           bool OneClass(DPS op){if(Class==op.Class)return true; else return false;}
           unsigned int damag(){return damages;}
           double reskill(){return skill;}
           double recrit(){return crit;}
           double critskill(){return ((crit/skill)*100);}
           int len(){ int i; i=strlen(name); return i;}
           friend ostream &operator << (ostream &stream, DPS obj);
};

bool DPS::Ignore()//игнорирует мобов
{
	char ignore[512];
	ifstream init;
	init.open("monsters.txt"); 

	while (init)
		{
			init.getline(ignore,510);
			if(strstr(ignore,name)){return true;}
		}
		
	init.close();
return false;
}

bool DPS::operator==(DPS op2)
{
        if(strcmp(name,op2.name))
        return false;
        else
        return true;
}        

ostream &operator <<(ostream &stream, DPS obj)
{
	if(obj.Class==1)stream << "страж - ";
	if(obj.Class==2)stream << "гладиатор - ";
	if(obj.Class==3)stream << "убийца - ";
	if(obj.Class==4)stream << "стрелок - ";
	if(obj.Class==5)stream << "волшебник - ";
	if(obj.Class==6)stream << "заклинатель - ";
	if(obj.Class==7)stream << "целитель - ";
	if(obj.Class==8)stream << "чародей - ";
	if(obj.Class==9)stream << "бард - ";
	if(obj.Class==10)stream << "снайпер - ";
	if(obj.Class==11)stream << "пилот - ";
/*	if((obj.crit/obj.skill*100)>0)
	{
		stream << obj.name << ":" << obj.damages << " (крит " << (obj.crit/obj.skill*100) << "%)"; //<<"-" <<obj.Class;//для вывода класса
	}
	else
	{
*/		stream << obj.name << ":" << obj.damages; //<<"-" <<obj.Class;//для вывода класса
//	}
        stream <<endl;       
        return stream;       
}

bool DPS::Dota()//определение доты
{
    int i=0;
	char dota[512]="";
	ifstream init;
    init.open("dots.txt"); 
    
	while (init)
		{
		  	init.getline(dota,510);
		  	if(strstr(dota,"class"))i++;
		    if(strstr(dota,name)&&(strlen(dota)==strlen(name))){Class = i; return true;}
		}
		
	init.close();
return false;
}

void DPS::Classes(char *str)// определение класса
{
	int i=0;
    bool b=1;
    char name1[128]="";
    char name2[128]="";
    char skill[512]="";
    ifstream init;
        
    if(strstr(str,"Критический удар! "))
    {
    	str=(strstr(str,"Критический удар! ")+18);
    }
     else       
    if(strstr(str,"Критический удар!"))
    {
    	str=(strstr(str,"Критический удар!")+17);
    }
    
    while(b)
       {
	         name1[i]=*str;
             i++;
             str++;
             if(strstr(name1,".")||strstr(name1,":")) {b=0;}
             if(i==100)b=0;
       }
       
    i--;
    if(strstr(name1," ."))i--;
    strncpy(name2,name1,i);
    i=0;

    init.open("skill.txt"); 
    
	while (init)
		{
		  	init.getline(skill,510);
		  	if(strstr(skill,"class"))i++;
		    if(strstr(skill,name2)&&(strlen(skill)==strlen(name2))){Class = i; break;}
		}
		
	init.close();
}

void DPS::Clocker(char *str)//скилы что не прописывают имя атакера
{
	char name3[128]="";
	char name4[128]="";
    int i=0;
    bool b=1;
	    
	    while(b)//!isspace(*str)
        {
	         name3[i]=*str;
             i++;
             str++;
             if(strstr(name3,".")) {b=0;}
             if(i==100)b=0;
        }
    i--;
    strncpy(name4,name3,i);
    strcpy(name,name4);
}

DPS NameDPS(char *str)//находим имя атакера или название скила
{
    DPS Ambrosia;
    char Name1[128]="";
    char name[128]="";
    int i=0;
    bool b=1;
    str = (strstr (str, " : ")+3);
   
   if(strstr (str, "Критический удар!")) str = (strstr (str, "Критический удар!")+17);// 16.04.2017
    while(b)
    {
            while(!isspace(*str))
            {
                       name[i]=*str;
                       str++;
                       i++;
            }
           if(i>0)
           {
              if(strncmp(name,"Критический",8)&&strncmp(name,"удар!",5))
                     {
                          if(!strncmp(name," наносит",8)||strstr(name,"использует")){b=0;}
                           else
                           {
                              if(strstr(name,":"))
							  {
							  	b=0;
							  	i--;
							  	strncat(Name1,name,i);
							  }
							  else
							  {
							  	if(strstr(name,"получает")||strstr(name,"отражает"))
								  {
								  	b=0;
								  }
								  else
								  {								  
							        strncat(Name1,name,i);
							      }
						      }
                              i=1;
                           }
                     }       
                     else
                     {
                        i=0;
                     }
           }         
    str++;
    strcpy(name," ");
    }
    
    Ambrosia.putn(Name1);
return Ambrosia; 
}

DPS seachDPS (char *str)//определение типа урона
{
    DPS Ambrosia;

    if(strstr (str, "наносит"))//авта других
         {
              Ambrosia=NameDPS(str);
              str=(strstr(str,"наносит")+8);
              Ambrosia.putd(Damag(str));
         }
        
    if(strstr (str, "получает")) 
     {
              if(strstr(str,"использует"))//умения других
              {
               Ambrosia=NameDPS(str);
               str=(strstr(str,"получает")+9);
               Ambrosia.putd(Damag(str));
              }
              else
              {
                if(strstr(str,"от умения"))//умения что не прописывают имя атакера
                 {
                   str=(strstr(str,"получает")+9);
                   Ambrosia.putd(Damag(str));     
                   str=(strstr(str,"от умения")+10);
                   Ambrosia.Clocker(str);   
                 }
                 else
                 {
                   if(strstr(str,"вместо"))//замена или зс(?)
                    {
                      Ambrosia=NameDPS(str);
                      str=(strstr(str,"ед.")-1);
                      while(!isalpha(*str))
                         {
                           str--;
                         }
                      str++;
                      Ambrosia.putd(Damag(str));                                                              
                    }
                    else
                    {
                      if(!Ambrosia.len()){Ambrosia=NameDPS(str);}
                      if(!Ambrosia.Dota())//доты или мои скилы
                        {
                           Ambrosia.putn(::myname); //для проверки дот закомментировать
                           Ambrosia.putskill(1);
                           if(strstr(str, "Критический удар!")) Ambrosia.putcrit(1);
                        }
                    str=(strstr(str,"получает")+9);
                    if(!strstr(str,"["))
                    Ambrosia.putd(Damag(str));
                    }
                 }
              }
     }
     
    if(strstr (str, "нанесли"))//моя авта
     {         
              Ambrosia.putn(::myname);
              Ambrosia.putskill(1);
              if(strstr(str, "Критический удар!")) Ambrosia.putcrit(1);
              str=(strstr(str,"нанесли")+8);
              Ambrosia.putd(Damag(str));
     }
     
return Ambrosia;
}

/*--------------------------------------Windows function--------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
setlocale(LC_ALL, "rus");
PAINTSTRUCT ps;
HDC hdc;
static TCHAR name[256];
static TCHAR nameSaveD[256]=_T("DPS.txt");
static TCHAR nameSaveH[256]=_T("Heal.txt");
static OPENFILENAME file, fileSave;
ifstream in;
ofstream out;
static vector<string> v;
vector<string>::iterator it;
string st;
int y, k;
static int n,length,sx,sy,cx,iVscrollPos,iHscrollPos,COUNT,MAX_WIDTH,skille,keySave;
static SIZE size = {8, 16};
char buff[1024];
char *Buff;
DPS AmbrosiaDPS;
DPS AmvrDPS;
DPS MAXDPS;
Heal AmbrosiaHeal;
Heal AmvrHeal;
Heal MAXHeal;
vector<DPS> WhiteDamages;
vector<DPS> BlackDamages;
vector<Heal> Healer;

  switch(Message) 
  {
        case WM_CREATE:
        {
          in.open("data.ini");
          in.getline(buff,510);
          strcpy(name,buff);
          in.close();
		  file.lStructSize = sizeof(OPENFILENAME);
		  file.lpstrFilter = _T("Chat\0*.log");
		  file.lpstrFile = name;
		  file.nMaxFile = 256;
		  file.lpstrInitialDir = _T(".\\");
		  file.lpstrDefExt = _T("log");
		  
		  fileSave.lStructSize = sizeof(OPENFILENAME);
		  fileSave.lpstrFilter = _T("*.txt");
		  fileSave.lpstrFile = nameSaveD;
		  fileSave.nMaxFile = 256;
		  fileSave.lpstrInitialDir = _T(".\\");
		  fileSave.lpstrDefExt = _T("txt");
		break;
	    }
		
		case WM_SIZE:
		{
		  sx = LOWORD(lParam);
		  sy = HIWORD(lParam);
		  k = n - sy/size.cy;
		  if (k > 0) COUNT = k; else COUNT = iVscrollPos = 0;
		  SetScrollRange(hwnd, SB_VERT, 0, COUNT, FALSE);
		  SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE);
		  k = length - sx/size.cx;
		  if (k > 0) MAX_WIDTH = k; else MAX_WIDTH = iHscrollPos = 0;
		  SetScrollRange(hwnd, SB_HORZ, 0, MAX_WIDTH, FALSE);
		  SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
		break;
	    }
		
		case WM_MOUSEWHEEL:
		{
		  iVscrollPos -= (short)HIWORD(wParam)/WHEEL_DELTA;
		  iVscrollPos = max(0, min(iVscrollPos, COUNT));
		  SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
		  InvalidateRect(hwnd, NULL, TRUE);
	    break;
	    }
		
		case WM_VSCROLL :
		{
		  switch(LOWORD(wParam))
		  {
		   case SB_LINEUP : iVscrollPos--; break;
		   case SB_LINEDOWN : iVscrollPos++; break;
		   case SB_PAGEUP : iVscrollPos -= sy / size.cy; break;
		   case SB_PAGEDOWN : iVscrollPos += sy / size.cy; break;
		   case SB_THUMBPOSITION : iVscrollPos = HIWORD(wParam); break;
		  }
		  iVscrollPos = max(0, min(iVscrollPos, COUNT));
		  if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))
		  {
		   SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
		   InvalidateRect(hwnd, NULL, TRUE);
		  }
	    break;
	    }
		
		case WM_HSCROLL :
		{
		  switch(LOWORD(wParam))
		  {
		    case SB_LINEUP : iHscrollPos--; break;
			case SB_LINEDOWN : iHscrollPos++; break;
			case SB_PAGEUP : iHscrollPos -= 8; break;
			case SB_PAGEDOWN : iHscrollPos += 8; break;
			case SB_THUMBPOSITION : iHscrollPos = HIWORD(wParam); break;
		  }
		  iHscrollPos = max(0, min(iHscrollPos, MAX_WIDTH));
		  if (iHscrollPos != GetScrollPos(hwnd, SB_HORZ))
		  {
		    SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);
		  }
		break;        
	    }
		
		case WM_COMMAND:
		  switch (LOWORD(wParam))
		  {
		  	case 0 :
		  	{
			  file.lpstrTitle = _T("Открыть файл для чтения");
			  file.Flags = OFN_HIDEREADONLY;
			  if (!GetOpenFileName(&file)) return 1;
			  if (!v.empty()) std::vector<std::string>().swap(v);
			  n = length = 0;
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  SetCurrentDirectory(buffer);
			  out.open("data.ini");
			  out << name;
			  out.close();		  
			break;
		    }
		    
	        case 1 :
		    {
		      if (v.empty()) {MessageBox(hwnd, "Пусто!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
		      fileSave.lpstrTitle = _T("Открыть файл для записи");
			  fileSave.Flags = OFN_NOTESTFILECREATE;
			  if(keySave==1)
			  {
			    if (!GetSaveFileName(&fileSave)) return 1;
			  	out.open(nameSaveD);
			    for (it = v.begin(); it != v.end(); ++it) out << *it << '\n';
			    out.close();
		      }
              if(keySave==2)
			  {
			    fileSave.lpstrFile = nameSaveH;
			    if (!GetSaveFileName(&fileSave)) return 1;
			    out.open(nameSaveH);
			    for (it = v.begin(); it != v.end(); ++it) out << *it << '\n';
			    out.close();
			  }
			break;
	        }   
				  
		    case 2: 
		    {
			  PostQuitMessage(0);
			break;
	    	}      
	    	
		    case 3 :/*--------------------------------------DPS---------------------------------------------------------*/
			{
			  SetCurrentDirectory(buffer);
			  in.open("name.ini");
			  	in.getline(buff,510);
			  	strcpy(::myname,buff); 	
			  in.close();
			  if (!v.empty()) std::vector<std::string>().swap(v);
			  n = length = 0;
			  keySave = 1;
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  in.open(name); 
			  if (!in.is_open()) {MessageBox(hwnd, "Chat.log не найден!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
			  while(in)
			  {
			    in.getline(buff,510);
			    if(strstr(buff,"урона"))
			    {
			    AmbrosiaDPS=seachDPS(buff);	
				if(!AmbrosiaDPS.Ignore())
				{	    
                if(AmbrosiaDPS.len())
                {
                  if(AmbrosiaDPS.Dota())
                  {
                    bool cat=1;
                    if(BlackDamages.size())
                    {
                      for(register unsigned int i=0; i < BlackDamages.size(); i ++)//вектор дотов
                      {
                        AmvrDPS=BlackDamages[i];
                        if(AmbrosiaDPS==AmvrDPS)
                        {
                          AmvrDPS.putd(AmbrosiaDPS.damag());
                          BlackDamages[i]=AmvrDPS;                   
                          cat = 0;
                        }
                      }
                    }
                    if(cat)
                    {
                      if(AmbrosiaDPS.damag()!=0)
                      {
                        BlackDamages.insert(BlackDamages.end(),AmbrosiaDPS);
                      }
                    }                   
                  }
                  else
                  {
                    bool cat=1;
                    if(WhiteDamages.size())
                    {
                      for(register unsigned int i=0; i < WhiteDamages.size(); i ++)//вектор игроков
                      {
                        AmvrDPS=WhiteDamages[i];
                        if(AmbrosiaDPS==AmvrDPS)
                        {
                          AmvrDPS.putd(AmbrosiaDPS.damag());
                          AmvrDPS.putskill(AmbrosiaDPS.reskill());
                          AmvrDPS.putcrit(AmbrosiaDPS.recrit());
                          WhiteDamages[i]=AmvrDPS;                   
                          cat = 0;
                        }
                      }
                    }
                    if(cat)
                    {
                      if(AmbrosiaDPS.damag()!=0)
                      {
                        WhiteDamages.insert(WhiteDamages.end(),AmbrosiaDPS);
                      }
                    }
                  }//else
                }//if len
                }//if ignore
			    }
              }//while
              in.close();

/*-----------------------------определение класса------------------------------------------------------*/
              for(register unsigned int j=0; j < WhiteDamages.size(); j ++)
			  {
			  	int i=0;
			    	AmbrosiaDPS=WhiteDamages[j];
                    in.open(name);
                    while(in)
			        {
			  	      in.getline(buff,510);
			  	      if(strstr(buff,"урона"))
			          {
                      AmvrDPS=seachDPS(buff);
                      if(AmbrosiaDPS==AmvrDPS)
                      {
                      	if(AmvrDPS.MyName())
                      	{
						  if(strstr(buff," : "))
						  {
							Buff=(strstr(buff," : ")+3);
						    AmbrosiaDPS.Classes(Buff);
						    if(!AmbrosiaDPS.TryClass())
						    {
						        i++;
						    	WhiteDamages[j]=AmbrosiaDPS;
						    }
                      	  }
                        }
                        else
                        {
                        if(strstr(buff,"использует:"))
						{
							Buff=(strstr(buff,"использует:")+12);
						    AmbrosiaDPS.Classes(Buff);
						    if(!AmbrosiaDPS.TryClass())
						    {
						        i++;
						    	WhiteDamages[j]=AmbrosiaDPS;
						    }
                      	}
                      	else
                      	{
                      	if(strstr(buff,"использует "))
						{
							Buff=(strstr(buff,"использует ")+11);
						    AmbrosiaDPS.Classes(Buff);
						    if(!AmbrosiaDPS.TryClass())
						    {
						        i++;
						    	WhiteDamages[j]=AmbrosiaDPS;
						    }
                      	}
                      	}
                        }
                      }
                     if(i==1)break;
			        }
			    }
                    in.close();
              }

/*---------------------------------------прибавка дот по классам------------------------------------------------*/        
              for(register unsigned int j=0; j < BlackDamages.size(); j ++)
			  {
			     AmbrosiaDPS=BlackDamages[j];
			     AmbrosiaDPS.Dota();
			     BlackDamages[j]=AmbrosiaDPS;
              }
              for(register unsigned int j=0; j < WhiteDamages.size(); j ++)
              {
              	skille=0;
              	AmbrosiaDPS=WhiteDamages[j];
              	for(register unsigned int i=0; i < WhiteDamages.size(); i ++)
              	{
              		AmvrDPS=WhiteDamages[i];
              		if(AmbrosiaDPS.OneClass(AmvrDPS))
					  {
              		    skille++;
              	      }
              	}
              	for(register unsigned int k=0; k < BlackDamages.size(); k ++)
              	{
              		AmvrDPS=BlackDamages[k];
              		if(AmbrosiaDPS.OneClass(AmvrDPS))
              		{
              			AmbrosiaDPS.putd((AmvrDPS.damag()/skille));
              		}
              	}
              	WhiteDamages[j]=AmbrosiaDPS;
              }

/*---------------------------------------Сортировка DPS------------------------------------------------*/  
              for(register unsigned int j=0; j < WhiteDamages.size(); j ++)
              { 
                MAXDPS=WhiteDamages[j];
                for(register unsigned int i=j; i < WhiteDamages.size(); i ++)
                {
                  AmbrosiaDPS=WhiteDamages[i];
                  if(AmbrosiaDPS.damag()>MAXDPS.damag())
                  {
                    MAXDPS=AmbrosiaDPS;
                    AmbrosiaDPS=WhiteDamages[j];
                    WhiteDamages[j]=MAXDPS;
                    WhiteDamages[i]=AmbrosiaDPS;
                  }
                }
              }
              for(register unsigned int j=0; j < BlackDamages.size(); j ++)
              {
                 MAXDPS=BlackDamages[j];
                 for(register unsigned int i=j; i < BlackDamages.size(); i ++)
                 {
                   AmbrosiaDPS=BlackDamages[i];
                   if(AmbrosiaDPS.damag()>MAXDPS.damag())
                   {
                     MAXDPS=AmbrosiaDPS;
                     AmbrosiaDPS=BlackDamages[j];
                     BlackDamages[j]=MAXDPS;
                     BlackDamages[i]=AmbrosiaDPS;
                   }
                 }
              }
/*---------------------------------------запись в файл------------------------------------------------*/                    	
			  SetCurrentDirectory(buffer);              
              out.open("DPS.txt");
              int mesto=1;
              bool EU=0;
              bool GS=0;
			  for(register unsigned int i=0; i < WhiteDamages.size(); i ++)
              {
//              	AmbrosiaDPS=WhiteDamages[i];
//				if(!AmbrosiaDPS.TryClass())
//				{
					out << mesto;
				    out << ")";
					out << WhiteDamages[i];
					mesto++;
//				}
              }
              for(register unsigned int i=0; i < BlackDamages.size(); i ++)
              {
                AmbrosiaDPS=BlackDamages[i];
                if(AmbrosiaDPS.GSClass())
                {
				    GS=1;
                }
                if(AmbrosiaDPS.EUClass())
                {
				   EU=1;
                }
			  }
			  if(GS)
			  {
			  out << "\n==============гс================\n";    
			  for(register unsigned int i=0; i < BlackDamages.size(); i ++)
              {
                AmbrosiaDPS=BlackDamages[i];
                if(AmbrosiaDPS.GSClass())
                {
				   out << " " << BlackDamages[i];
                }
			  } 
			  } 
			  if(EU)
			  {
			  out << "\n=========Союз стихий============\n";    
			  for(register unsigned int i=0; i < BlackDamages.size(); i ++)
              {
                AmbrosiaDPS=BlackDamages[i];
                if(AmbrosiaDPS.EUClass())
                {
				   out << " " << BlackDamages[i];
                }
			  } 
			  }

			  out << "================================";                             
			  out << "\n Ваша крит. атака - ";    			  
			   for(register unsigned int i=0; i < WhiteDamages.size(); i ++)
              {
              	AmbrosiaDPS=WhiteDamages[i];
                if(AmbrosiaDPS.MyName())
                {
                	out << AmbrosiaDPS.critskill();
                	out << "%";
                }
			  }
			  out.close();
              in.open("DPS.txt");//чтение файла Damages.txt и запись его в вектор v 
			  while (getline(in,st))
			  {
			    if (length < st.length()) length = st.length();
				v.push_back(st);
			  }
			  in.close();
			  n = v.size();
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  MessageBox(hwnd, "Готово!", "DPS", MB_OK | MB_ICONINFORMATION);
            break;
			}
/*--------------------------------------DPS-close-------------------------------------------------------*/
			case 7://доты
				{
			  in.open(name); 
			  if (!in.is_open()) {MessageBox(hwnd, "Chat.log не найден!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
			  while(in)
			  {
			    in.getline(buff,510);
			    if(strstr(buff,"урона"))
			    {
			    AmbrosiaDPS=seachDPS(buff);	
                if(AmbrosiaDPS.len())
                {
                  if(AmbrosiaDPS.Dota())
                  {
                    bool cat=1;
                    if(BlackDamages.size())
                    {
                      for(register unsigned int i=0; i < BlackDamages.size(); i ++)//вектор дотов
                      {
                        AmvrDPS=BlackDamages[i];
                        if(AmbrosiaDPS==AmvrDPS)
                        {
                          AmvrDPS.putd(AmbrosiaDPS.damag());
                          BlackDamages[i]=AmvrDPS;                   
                          cat = 0;
                        }
                      }
                    }
                    if(cat)
                    {
                      if(AmbrosiaDPS.damag()!=0)
                      {
                        BlackDamages.insert(BlackDamages.end(),AmbrosiaDPS);
                      }
                    }                   
                  }
                }//if len
			    }
              }//while
              in.close();
              
                            for(register unsigned int j=0; j < BlackDamages.size(); j ++)
			  {
			     AmbrosiaDPS=BlackDamages[j];
			     AmbrosiaDPS.Dota();
			     BlackDamages[j]=AmbrosiaDPS;
              }
                            for(register unsigned int j=0; j < BlackDamages.size(); j ++)
              {
                 MAXDPS=BlackDamages[j];
                 for(register unsigned int i=j; i < BlackDamages.size(); i ++)
                 {
                   AmbrosiaDPS=BlackDamages[i];
                   if(AmbrosiaDPS.damag()>MAXDPS.damag())
                   {
                     MAXDPS=AmbrosiaDPS;
                     AmbrosiaDPS=BlackDamages[j];
                     BlackDamages[j]=MAXDPS;
                     BlackDamages[i]=AmbrosiaDPS;
                   }
                 }
              }
			  out.close();
			  out.open("DPS.txt");
			  out << "============ДОТЫ===============\n";    
			  for(register unsigned int i=0; i < BlackDamages.size(); i ++)
              {
                out << BlackDamages[i];
			  }                                               
			  out.close();
				   if (!v.empty()) std::vector<std::string>().swap(v);
			       n = length = 0;
			       in.open("DPS.txt");//чтение файла Damages.txt и запись его в вектор v 
			       if (!in.is_open()) {MessageBox(hwnd, "Сначала расчитайте!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
			       while (getline(in,st))
			       {
			         if (length < st.length()) length = st.length();
			        	v.push_back(st);
			       }
			       in.close();
			       n = v.size();
			       SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			       InvalidateRect(hwnd, NULL, TRUE);
			       MessageBox(hwnd, "Готово!", "DOT", MB_OK | MB_ICONINFORMATION);
				break;
				}
/*--------------------------------------поиск дот у замерщика-------------------------------------------------------*/				
				case 8:
				{
			  if (!v.empty()) std::vector<std::string>().swap(v);
			  n = length = 0;
			  keySave = 1;
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  in.open(name); 
			  if (!in.is_open()) {MessageBox(hwnd, "Chat.log не найден!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
			  while(in)
			  {
			    in.getline(buff,510);
			  if(strstr (buff, "получает")) 
              {
              if(strstr(buff,"использует"))//умения других
              {
              	
              }
              else
              {
                if(strstr(buff,"от умения"))//умения что не прописывают имя атакера
                 {

                 }
                 else
                 {
                   if(strstr(buff,"вместо"))//замена или зс(?)
                    {
                                                            
                    }
                    else
                    {
     		    if(strstr(buff,"урона")&&strstr(buff,"получает"))
			    {
			        AmbrosiaDPS=NameDPS(buff);
                    Buff=(strstr(buff,"получает")+9);
                    AmbrosiaDPS.putd(Damag(Buff));
                if(!AmbrosiaDPS.Dota())
				{	    
                if(AmbrosiaDPS.len())
                {
                    bool cat=1;
                    if(WhiteDamages.size())
                    {
                      for(register unsigned int i=0; i < WhiteDamages.size(); i ++)
                      {
                        AmvrDPS=WhiteDamages[i];
                        if(AmbrosiaDPS==AmvrDPS)
                        {
                          AmvrDPS.putd(AmbrosiaDPS.damag());
                          AmvrDPS.putskill(AmbrosiaDPS.reskill());
                          AmvrDPS.putcrit(AmbrosiaDPS.recrit());
                          WhiteDamages[i]=AmvrDPS;                   
                          cat = 0;
                        }
                      }
                    }
                    if(cat)
                    {
                      if(AmbrosiaDPS.damag()!=0)
                      {
                      	Buff=(strstr(buff," : ")+3);
                      	AmbrosiaDPS.Classes(Buff);
                        WhiteDamages.insert(WhiteDamages.end(),AmbrosiaDPS);
                      }
                    }
                }//if len
                }//if ignore
                }
                                    }
                 }
              }
              }
              }//while
              in.close();

/*---------------------------------------Сортировка DPS------------------------------------------------*/  
              for(register unsigned int j=0; j < WhiteDamages.size(); j ++)
              { 
                MAXDPS=WhiteDamages[j];
                for(register unsigned int i=j; i < WhiteDamages.size(); i ++)
                {
                  AmbrosiaDPS=WhiteDamages[i];
                  if(AmbrosiaDPS.damag()>MAXDPS.damag())
                  {
                    MAXDPS=AmbrosiaDPS;
                    AmbrosiaDPS=WhiteDamages[j];
                    WhiteDamages[j]=MAXDPS;
                    WhiteDamages[i]=AmbrosiaDPS;
                  }
                }
              }
/*---------------------------------------запись в файл------------------------------------------------*/                    	
			  SetCurrentDirectory(buffer);              
              out.open("DPS.txt");
			  for(register unsigned int i=0; i < WhiteDamages.size(); i ++)
              {
              	AmbrosiaDPS=WhiteDamages[i];
              	if(AmbrosiaDPS.TryClass())
              	{
              		out << WhiteDamages[i];
              	}
              }                       
			  out.close();
              in.open("DPS.txt");//чтение файла Damages.txt и запись его в вектор v 
			  while (getline(in,st))
			  {
			    if (length < st.length()) length = st.length();
				v.push_back(st);
			  }
			  in.close();
			  n = v.size();
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  MessageBox(hwnd, "Готово!", "DOT SEARCH", MB_OK | MB_ICONINFORMATION);
            break;
			}
/*--------------------------------------HEAL----------------------------------------------------------*/
			case 4:
			{
			  if (!v.empty()) std::vector<std::string>().swap(v);
			  n = length = 0;
			  keySave=2;
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  in.open(name); 
			  if (!in.is_open()) {MessageBox(hwnd, "Chat.log не найден!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
			  
			  while(in)
			  {
                in.getline(buff,510);
                AmbrosiaHeal=seachHeal(buff);
                if(!AmbrosiaHeal.IgnoreHeal())
                {
				if(AmbrosiaHeal.len())
                {
                  bool cat=1;
                  if(Healer.size())
                  {
                    for(register unsigned int i=0; i < Healer.size(); i ++)
                    {
                      AmvrHeal=Healer[i];
                      if(AmbrosiaHeal==AmvrHeal)
                      {
                        AmvrHeal.puth(AmbrosiaHeal.hp());
                        AmvrHeal.putm(AmbrosiaHeal.mp());                                             
                        Healer[i]=AmvrHeal;  
                        cat = 0;                 
                      }
                    }
                  }
                  if(cat)
                  {
				    if(AmbrosiaHeal.hp()!=0||AmbrosiaHeal.mp()!=0)
                    {
                      Healer.insert(Healer.end(),AmbrosiaHeal);
                    }
                  }//if cat
                }//if len
                }//if ignore
              }//while
              in.close();           
/*---------------------------------------Сортировка Heal------------------------------------------------*/  
              for(register unsigned int j=0; j < Healer.size(); j ++)
			  {
                MAXHeal=Healer[j];
                for(register unsigned int i=j; i < Healer.size(); i ++)
                {
                  AmbrosiaHeal=Healer[i];
                  if(AmbrosiaHeal.hp()>MAXHeal.hp())
                  {
                    MAXHeal=AmbrosiaHeal;
                    AmbrosiaHeal=Healer[j];
                    Healer[j]=MAXHeal;
                    Healer[i]=AmbrosiaHeal;
                  }
                }
              }
/*---------------------------------------Запись в файл------------------------------------------------*/  
			  SetCurrentDirectory(buffer);
			  out.open("DPS.txt");  
			  out <<"==========Отхил==========\n";
			  for(register unsigned int i=0; i < Healer.size(); i ++)
			  {
                AmbrosiaHeal=Healer[i];
				if(AmbrosiaHeal.hp()) 
				{
				  out << i+1;
				  out << " ";
			      out << "Место: ";
				  out << Healer[i];
				  out << "\n";
                }
              }    
			  for(register unsigned int j=0; j < Healer.size(); j ++)//сортировка по мп
			  {
                MAXHeal=Healer[j];
                for(register unsigned int i=j; i < Healer.size(); i ++)
                {
                  AmbrosiaHeal=Healer[i];
                  if(AmbrosiaHeal.mp()>MAXHeal.mp())
                  {
                    MAXHeal=AmbrosiaHeal;
                    AmbrosiaHeal=Healer[j];
                    Healer[j]=MAXHeal;
                    Healer[i]=AmbrosiaHeal;
                  }
                }
              }
			  out <<"=========Залив маны======\n";
			  for(register unsigned int i=0; i < Healer.size(); i ++)
			  {
			    AmbrosiaHeal=Healer[i];
				if(AmbrosiaHeal.mp())
				{
				  out << i+1;
				  out << " ";
				  out << "Место: ";
				  out << Healer[i];
				  out << "\n";
				}
			  }    
			  out.close();
			  
			  in.open("DPS.txt");//===========чтение файла Heal.txt и запись его в вектор v 
			  while (getline(in,st))
			  {
			    if (length < st.length()) length = st.length();
				v.push_back(st);
			  }
			  in.close();
			  n = v.size();
			  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			  InvalidateRect(hwnd, NULL, TRUE);
			  MessageBox(hwnd, "Готово!", "HP MP", MB_OK | MB_ICONINFORMATION);
            break;
            }
/*--------------------------------------HEAL-close--------------------------------------------------------*/

	        case 5 ://Копировать
	      	{
	      	  if (!v.empty())
			  {
	      	  char str[n*length];
	      	  int k=0;
	      	  for(int i=0; i<n; i++)
	      	  {
	      	  	st=v[i];
	      	  	for(int j=0; j<st.size(); j++)
	      	  	{
	      	  		str[k]=st[j];
	      	  		k++;
	      	  	}
	      	  	str[k]='\n';
	      	  	k++;
	      	  }
              OpenClipboard(0);
              EmptyClipboard();
              HGLOBAL hStrMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(str));
              void* pStrMem = GlobalLock(hStrMem);
              memcpy(pStrMem, str, sizeof(str));
              GlobalUnlock(pStrMem);
              SetClipboardData(CF_TEXT, hStrMem);
              CloseClipboard();
              }
	         break;
	        }
	        
	        case 6 ://Очистка
		    {
			    if (!v.empty()) std::vector<std::string>().swap(v);
			    n = length = 0;
			    SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			    InvalidateRect(hwnd, NULL, TRUE);
			  	out.open(name);
			    out.close();
			break;
	        }
			case 9://опыт
			   {
			   	bool taimer=1;
			   	unsigned int h=0;
			   	unsigned int m=0;
			   	unsigned int s=0;
			   	unsigned int timesum=0;
			   	unsigned int timesum2=0;
			   	unsigned int chetchik=0;
			   	unsigned int chetchik2=0;
			   	if (!v.empty()) std::vector<std::string>().swap(v);
				  n = length = 0;
				  keySave = 1;
				  SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
				  InvalidateRect(hwnd, NULL, TRUE);
				  in.open(name); 
				  if (!in.is_open()) {MessageBox(hwnd, "Chat.log не найден!", "Ошибка", MB_OK | MB_ICONEXCLAMATION); return 1;}
				  while(in)
				  {
				  		in.getline(buff,510);
				  		if(taimer)
				  		{
				  			Buff=(strstr(buff," : ")-8);
							h=Damag(Buff);	
							Buff=(strstr(buff," : ")-5);
							m=Damag(Buff);
							Buff=(strstr(buff," : ")-2);
							s=Damag(Buff);	
							timesum = (s + (m*60) + (h*3600));			  			
				  			taimer=0;
				  			
				  		}
			        	if(strstr(buff,"Вы получили")&&strstr(buff,"опыта"))
			    		{
			    			AmbrosiaDPS.putn(::myname); 
							Buff=(strstr(buff,"Вы получили")+12);
                            AmbrosiaDPS.putd(Damag(Buff));                    
							chetchik2++;      
				  		}
				  		chetchik++;
				  }
				  in.close();
				  in.open(name); 
				  while(in)
				  {
				  	chetchik--;
				  		in.getline(buff,510);
				  		if(chetchik==1)
				  		{
				  			Buff=(strstr(buff," : ")-8);
							h=Damag(Buff);	
							Buff=(strstr(buff," : ")-5);
							m=Damag(Buff);
							Buff=(strstr(buff," : ")-2);
							s=Damag(Buff);
							timesum2 = (s + (m*60) + (h*3600));				  			
				  		}
				  }
				  in.close();
				  timesum = timesum2 - timesum;
				  h = AmbrosiaDPS.damag();
				  if(timesum)
				  {
				  	m = h/timesum;
				  	timesum = timesum/60;
				  }
				  else
				  {
				  	m = h;
				  }
			 	  SetCurrentDirectory(buffer);              
            	  out.open("Exp.txt");
    			  out << h << " (" << m <<" опыта/в сек.) " << timesum <<" мин ";     
				  out << "\n" << "Кол-во моб./кв: " << chetchik2;     
			      out.close();
                  in.open("Exp.txt");//чтение файла Damages.txt и запись его в вектор v 
			 	  while (getline(in,st))
			  		{
			    		if (length < st.length()) length = st.length();
						v.push_back(st);
			 		}
			 	 in.close();
			 	 n = v.size();
			  	 SendMessage(hwnd, WM_SIZE, 0, sy << 16 | sx);
			     InvalidateRect(hwnd, NULL, TRUE);
			     MessageBox(hwnd, "Готово!", "Exp", MB_OK | MB_ICONINFORMATION);
            break;
			}
		  }
		  
		case WM_PAINT://выводим результат
		{
		  hdc = BeginPaint(hwnd, &ps);
		  for(y=0,it=v.begin()+iVscrollPos;it!=v.end()&&y<sy;++it,y+=size.cy)
		  if (iHscrollPos < it->length())
	      TabbedTextOutA(hdc,0,y,it->data()+iHscrollPos,it->length()-iHscrollPos,0,NULL,0);
		  EndPaint(hwnd, &ps);
	    }
		break;

		case WM_DESTROY: 
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
  }
return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wc; 
	HWND hwnd; 
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; 
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon         = LoadIcon(hInstance, "Ex4_Icon");
	wc.lpszMenuName  = "Ex4_Menu"; 
	wc.hIconSm		 = LoadIcon(hInstance,NULL); 

	if(!RegisterClassEx(&wc)) 
	{
	  MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
	  return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
	    "WindowClass",
		"Aion DPS Metr v1.3",
		WS_VISIBLE | WS_SYSMENU | WS_VSCROLL | WS_HSCROLL|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hwnd == NULL)
	{
	  MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
	  return 0;
	}
	
	GetCurrentDirectory(sizeof(buffer),buffer);
	
	while(GetMessage(&Msg, NULL, 0, 0) > 0) 
	{ 
	  TranslateMessage(&Msg); 
	  DispatchMessage(&Msg); 
	}
return Msg.wParam;
}
