/**
@file
 
Bu program kullanicininin belirledigi bir klasordeki
.txt dosyalarinda alt alta bulunan notlarin
ortalamasini almak uzere yazilmistir
 
@author
 
ID:                     Subtitle
 
Kullanilan compiler:    GCC
 
IDE:                    CodeBlocks 13.12
 
OS:                     Ubuntu 64-bit Oracle VM VirtualBox
*/

#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#define LINE_SIZE 8
#define THREADS 16
 
enum {
    WALK_OK = 0,
    WALK_BADPATTERN,
    WALK_BADOPEN,
};
 
void *average(void *fi){
    FILE *fc;
    int i, val, total = 0,  count = 0;
    int avr;
    char line[LINE_SIZE];
    //FILE *fc = (FILE *)fi;
    fc = fopen((char*)fi,"r");//.txt dosyasini burada aciyorum
    //Acilan .txt dosyasindaki notlarin ortalamasini bu while ile buluyorum
    while(fgets(line,LINE_SIZE-1,fc)){
        val = atoi(line);//Her satirdaki string'i burada int tipine donusturuyorum
        count++;//Kac satir not bilgisi girildigini bu degiskende tutuyorum
        total += val;//Notlar toplamini bu degiskende tutuyorum
 
    }
 
    avr = total/count;//Ve son olarak ortalamayi burada buluyorum
 
    //Bilgilerin kullaniciya yazdirilmasini burada yapiyorum
    printf("\nDosya adi:\n> ");
    puts(fi);
    printf("Not ortalamasi:\n> %d",avr);
 
    return NULL;
}
 
int walker(const char *dir, const char *pattern)
{
    int fileCount = 0,a;
    struct dirent *entry;
    regex_t reg;
    DIR *d;
    pthread_t thr[THREADS];
    if (regcomp(&reg, pattern, REG_EXTENDED | REG_NOSUB))
        return WALK_BADPATTERN;
    if (!(d = opendir(dir)))
        return WALK_BADOPEN;
 
    //Threadlerin olusturulmasi islemi bu while'da gerceklesiyor
    while (entry = readdir(d))
        if (!regexec(&reg, entry->d_name, 0, NULL, 0)){
 
            if(pthread_create(&thr[fileCount], NULL, &average, (void*)entry->d_name)){
                printf("Could not create thread %d\n", fileCount);
                return -1;
            }
            fileCount++;
        }
 
    //Acilan threadleri bu for dongusunde sonlandiriyorum
    for(a=0;a<fileCount;a++){
        if(pthread_join(thr[a], NULL)){
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    }
 
    closedir(d);
    regfree(&reg);
    return WALK_OK;
}
 
 
 
int main(int argc, char*argv[])
{
 
    char folderPath[128];
 
    printf("Lutfen notlarin bulundugu klasorun adresini giriniz\n> ");
    scanf("%s", folderPath);
 
 
    walker(folderPath, ".\\.txt$");
 
    printf("\n\nProgram sonlandiriliyor\n");
 
    return 0;
}
