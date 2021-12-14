#include <stdio.h> // printf, fprintf
#include <stdlib.h> // malloc, exit, execvp
#include <string.h> // strcmp, strtok
#include <sys/wait.h> // waitpid
#include <unistd.h> // fork, chdir, exec, pid_t
#include "shell.h"

static char* currentDirectory;
// built-in komutlar için fonksiyon başlıkları
int lsh_cd(char **args);
int lsh_showpid(char **args);
int lsh_exit(char **args);


void PromptBas() {
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
  // getcwd ile geçerli çalışma dizinini aldık.
	printf(YEL "%s@%s:"RESET RED "%s > " RESET, getenv("LOGNAME"), hostn, getcwd(currentDirectory, 1024));
}

// built-in komutları içeren dizi
char *builtin_str[] = {"cd", "showpid", "exit"};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_showpid,
  &lsh_exit
};
// built-in komut sayısı
int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

//Built-in fonksiyonlar

int lsh_cd(char **args) {
  // args[0] -> cd
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: Beklenmeyen argüman \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_showpid(char **args) {
  int i;
	int status;
	for (i=0;i<5;i++) { // 5 tane yavru process'in pid'sini yazdıracak olan döngü
		int pid=fork();
		if(pid==0) { // yavru process
			printf("%d\n",getpid()); // yavru process'in pid'si yazdırılıyor.
			exit(0);
		}
		else if(pid>0){ // ebeveyn process
			// waitpid
			pid = waitpid(pid, &status, 0); // yavru process'in işini beklemek için
		}
		else { // pid<0

		}
  }
  return 1; // programın sonlanmaması için
}

// exit komutuyla shell'e exit yazdırılıp uygulama sonlandırılıyor.
int lsh_exit(char **args) {
	int status;
	while (!waitpid(-1,&status,WNOHANG)){} // WNOHANG-> zombie process oluşup oluşmadığını kontrol etmek için
  printf("exit\n"); 
	exit(0); // programın sonlanması için.
}