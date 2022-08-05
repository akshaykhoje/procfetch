#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include<bits/stdc++.h>
#include "color.h"
using namespace std;

string exec(string command) 
{
   char buffer[128];
   string result = "";
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }
   while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

string getuser()
{
	return getenv("USER");
}

string gethostname(string path)
{
	fstream fptr;
	fptr.open(path, ios::in);
	string hostname;
	getline(fptr, hostname);
	return hostname;
}

string getHost(string path)
{
    fstream f1, f2;
    string p1, p2, n1, n2;
    p1 = path + "product_name";
    p2 = path + "product_version";

    f1.open(p1, ios::in);
    getline(f1, n1);
    f1.close();

    f2.open(p2, ios::in);
    getline(f2, n2);
    f2.close();

    string host = n1 + " " + n2;

    return host;
}

string getUpTime(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
	string time;

	getline(fptr, time);
    time=time.substr(0, time.find(" "));

    int m = stoi(time)/60;
    int h = m/60;
    int d=h/24;

    string timeS;
    if(h==0)
    {
        timeS = to_string(m%60)+" mins";
    }
    else if(d==0)
    {
        timeS = to_string(h%24) + " hours, " + to_string(m%60)+" mins";
    }
    else
    {
        timeS = to_string(d) + " d, " + to_string(h%24) + " hours, " + to_string(m%60)+" mins";
    }

    return timeS;
}

string getCpu(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string cpu,line,sub;

    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,10);
        if(sub=="model name")
        {
            break;
        }
    }
    cpu=line.substr(line.find(":")+2);

    return cpu;
}

string getKernel(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string kernel;
    getline(fptr,kernel);
    return kernel;
}

string getRAM(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string line,sub,shmem;
    string total,free;
    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,line.find(":"));
        if(sub=="MemTotal")
        {
            total=line;
        }
        if(sub=="MemAvailable")
        {
            free=line;
        }
        if(sub=="Buffers")
        {
            shmem=line;
            break;
        }
    }
    int i;
    for(i=0;i<total.size();i++){
        if(isdigit(total[i])){
            break;
        }
    }
    total = total.substr(i);
    total = total.substr( 0 , total.find(" "));
    for(i=0;i<free.size();i++){
        if(isdigit(free[i])){
            break;
        }
    }
    free = free.substr(i);
    free = free.substr(0,free.find(" "));

    for(i=0;i<shmem.size();i++){
        if(isdigit(shmem[i])){
            break;
        }
    }

    shmem = shmem.substr(i);
    shmem = shmem.substr(0,shmem.find(" "));

    int memTotal = stoi(total);
    int memFree = stoi(free);
    int memAvail = (memTotal - memFree) - stoi(shmem);

    return to_string(memAvail/1024) + "MiB / " + to_string(memTotal/1024) + "MiB";
}

string getOS(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string line,sub;

    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,13);
        if(sub=="PRETTY_NAME=\"")
        {
            break;
        }
    }

    line = line.substr(line.find("\"")+1);
    line = line.substr(0,line.find("\""));

    return line;
}

string getHardwarePlatform()
{
    string s = exec("uname -m");
    s = s.substr(0,s.find("\n"));
    return " "+s;
}

string getSHELL(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string line,sub;
    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,line.find(":"));
        if(sub==getuser()){
            break;
        }
    }
    reverse(line.begin(),line.end());
    line=line.substr(0,line.find("/"));
    reverse(line.begin(),line.end());
    return line;
}

int getCPUtemp(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string temp;
    getline(fptr,temp);
    return stoi(temp);
}

string getDE()
{
    return getenv("XDG_CURRENT_DESKTOP");
}

string getRES(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string res;
    getline(fptr,res);
    res=res.substr(2);
    return res.substr(0,res.find("p"));
}


string getTheme()
{
    string theme = exec("gsettings get org.gnome.desktop.interface gtk-theme");
    theme=theme.substr(1);
    return theme.substr(0,theme.find("\'"));
}

string getIcons()
{
    string icon = exec(" gsettings get org.gnome.desktop.interface icon-theme");
    icon=icon.substr(1);
    return icon.substr(0,icon.find("\'"));
}

string getTerminal()
{
    return getenv("PATH");
}



vector<string> getGPU()
{   vector<string> gpu;
    string igpu = exec("lspci | grep -E  \"VGA|3D|Display\"");
    int temp=0,k=0;

    for(int i=0;i<igpu.size();i++){
        if(igpu[i]=='\n'){
            gpu.push_back(igpu.substr(temp,i-temp));
            gpu[k] = gpu[k].substr(gpu[k].find(": ")+2);
            gpu[k] = gpu[k].substr(0,gpu[k].find(" ("));
            temp=i+1;
            k++;
        }
    }
    return gpu;
} 


void print()
{
    string os = getOS("/etc/os-release");

    if(os.find("Ubuntu")!= string::npos){
        string path="../ascii/ubuntu.ascii";
        fstream fptr;
        fptr.open(path, ios::in);
        string txt;
        while(fptr)
        {
            getline(fptr,txt);
            cout<<BITAL<<txt<<endl;
        }
    }
}


string getPackages()
{
    string pkg="";
    int count=0;
    if(exec(" [ -f \"/bin/dpkg\" ] && echo \"1\"|wc -l  ").size() > 1){
        string dpkg = exec(" dpkg -l | wc -l ");
        pkg+= dpkg.substr(0,dpkg.size()-1) + RED+" dpkg, "+RESET;
    }
    if(exec(" [ -f \"/bin/snap\" ] && echo \"1\"|wc -l  ").size() > 1){
        string snap = exec(" snap list | wc -l ");
        pkg+= snap.substr(0,snap.size()-1) + RED+" snap, "+RESET;
    }
    if(exec(" [ -f \"/bin/pacman\" ] && echo \"1\"|wc -l  ").size() > 1){
        string pacman = exec(" pacman -Q | wc -l  ");
        pkg+= pacman.substr(0,pacman.size()-1) + RED+" pacman, "+RESET;
    }
    if(exec(" [ -f \"/bin/flatpak\" ] && echo \"1\"|wc -l  ").size() > 1){
        string flatpak = exec(" flatpak list | wc-l ");
        pkg+= flatpak.substr(0,flatpak.size()-1) + RED + " flatpak, "+RESET;
    }
    
    return pkg;
}

int main()
{
    print();
	string user = getuser();
	string hostname = gethostname("/etc/hostname");
    string username = YELLOW+user+RESET +"@"+YELLOW+hostname;
	cout <<username<<endl;
    for(int i=0;i<(user+hostname).size();i++){
        cout<<".";
    }
    cout<<endl;
    string HOST = getHost("/sys/devices/virtual/dmi/id/");
    cout<<GREEN<<"Host : "<<RESET<<HOST<<endl;
	string upTime = getUpTime("/proc/uptime");
    cout<<UNDERLINE<<GREEN<<"UpTime : "<<RESET<< upTime<<endl;
    string cpu = getCpu("/proc/cpuinfo");
    cout<<GREEN<<"CPU : "<<RESET<<cpu<< endl;
    string ram = getRAM("/proc/meminfo");
    cout<<GREEN<<"RAM : "<<RESET<<ram<< endl;
    string kernel = getKernel("/proc/sys/kernel/osrelease");
    cout<<GREEN<<"Kernel : "<<RESET<<kernel<< endl;
    string os = getOS("/etc/os-release");
    cout<<GREEN<<"OS : "<<RESET<<os<<getHardwarePlatform()<<endl;
    int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
    cout<<GREEN<<"CPU Temperature : "<<RESET<<float(temp/1000.0)<<" °C"<< endl;
    string shell = getSHELL("/etc/passwd");
    cout<<GREEN<<"shell : "<<RESET<<shell<< endl;
    string DE = getDE();
    cout<<GREEN<<"DE : "<<RESET<<DE<< endl;
    string res = getRES("/sys/class/graphics/fb0/modes");
    cout<<GREEN<<"Resolution : "<<RESET<<res<< endl;
    string theme  = getTheme();
    cout<<GREEN<<"Theme : "<<RESET<<theme<<endl;
    string icon = getIcons();
    cout<<GREEN<<"Icons : "<<RESET<<icon<< endl;
    vector<string> gpu = getGPU();
    for(auto it:gpu){
        cout<<GREEN"GPU : "<<RESET<<it<<endl;
    }
    string pkg  = getPackages();
    cout<<GREEN<<"Packages : "<<RESET<<pkg<<endl;
    return 0;
}
