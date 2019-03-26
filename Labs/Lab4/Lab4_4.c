#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void find(char* s, int n, int x) {

}


int main(int argc, char *argv[]) {
    int flagn = 0, flagx = 0, flagPattern = 0, flagFiles = 0;
    char *pattern;    
    for(int i = 1; i < argc; i++) {
        // printf("%d%dfile: %s\n",flagn,flagx, argv[i]);
        flagn = fmax(flagn,(argv[i][0] == '-' && (argv[i][1] == 'n' || (argv[i][2] != '\0' && argv[i][2] == 'n')) ? 1 : 0));
        flagx = fmax(flagx,(argv[i][0] == '-' && (argv[i][1] == 'x' || (argv[i][2] != '\0' && argv[i][2] == 'x')) ? 1 : 0));
        if(argv[i][0] == '-' && (argv[i][1] == 'n' || argv[i][1] == 'x' ))
            continue;
        // printf("Not flags\n");
        if(flagPattern == 1) {

            flagFiles = 1;
            FILE *fptr = fopen(argv[i],"r");
            if(fptr == NULL){
                // printf("OUT\n");
                flagFiles = 0;
                break;
            }

            // printf("File: %s\n",argv[i]);
            //find the pattern
            char line[1000];
            int lineNum = 1;
            int end = 0;

            while(1) {
                int x = 0;
                int patternFull = 0;
                int contPatteren = 0;
                if(fgets(line, 1000, fptr) == NULL) {
                    end = 1;
                    break;
                }
                for(int i = 0; i < 1000 && line[i] != '\n'; i++) {
                    
                    if(pattern[x] == line[i]){
                        contPatteren = 1;
                        x++;
                    }
                    else
                    {
                        contPatteren = 0;
                        x = 0;
                        patternFull = fmax(patternFull,0);
                    }
                    
                    if(pattern[x] == '\0') {
                        patternFull = 1;
                        x = 0;
                    }
                }
                //print the line

                if(flagn == 1 && ((flagx == 1 && patternFull == 0) || (flagx == 0 && patternFull == 1) )) {
                    printf("%d. ",lineNum, line);
                }

                if((flagx == 1 && patternFull == 0) || (flagx == 0 && patternFull == 1) ) {
                    printf("%s", line); 
                }
                                
                lineNum++;
            }

            // printf("Finish");

            
            fclose(fptr);
        }
        else{
            pattern = argv[i];
            // printf("find pattern\n");
            flagPattern = 1;
        }
    }
    if(1-flagFiles == 1) {
        //find the pattern from stdin
        for(int i = 0; argv[argc-1][0] != '\0'; i++){
            if(argv[argc-1][i] == pattern[i]){
                printf("Please provide a source for the finding tool\n");
                return 1;
            }
        }
        
            //find(argv[argc-1], flagn, flagx);
                
    }


    return 0;
}