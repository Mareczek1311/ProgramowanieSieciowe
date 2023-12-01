#include <stdio.h>
     #include <sys/types.h>
     #include <pwd.h>
     #include <uuid/uuid.h>
     #include <grp.h>
     #include <unistd.h>

int main(){

    struct passwd *pwdr;
    struct passwd *pwde;
    struct group *grr;    
    struct group *gre;    
    int real_uid, effective_uid, real_gid, effective_gid;
    
    real_uid = getuid();
    effective_uid = geteuid();

    real_gid = getgid();
    effective_gid = getegid();

    pwdr = getpwuid(real_uid);
    pwde = getpwuid(effective_uid);
    

    grr = getgrgid(real_gid);
    gre = getgrgid(effective_gid);
   
    printf("Rzeczywisty uzytkownik (id, nazwa): %d, %s \n", pwdr->pw_uid, pwdr->pw_name);
    printf("Rzeczywista grupa (id, nazwa): %d, %s \n", grr->gr_gid, grr->gr_name);
    printf("Obowiazujacy uzytkownik (id, nazwa): %d, %s \n", pwde->pw_uid, pwde->pw_name);
    printf("Obowiazujaca grupa (id, nazwa): %d, %s \n", gre->gr_gid, gre->gr_name);

    return 0;
}
