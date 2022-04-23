/* ----------------------------------------------------------
 * file: test3.c
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * ----------------------------------------------------------
 * function: a simple class-choosing system
 * description: according to the given message provids credit
 * 				 data and advice
 * ----------------------------------------------------------
 * create date:   2022/4/15 4:01 pm
 * last modified: 2022/4/15 10:16 pm
 * ----------------------------------------------------------
 * history: 2022/4/15 4:01 pm create the file
 *          2022/4/15 10:16 pm finish the codes
 */
// #include <stdio.h>
// #include <string.h>

//varibles' definition
char name[100][6];
int  credit[100] = {0};
char precourse[100][300];
char grade[100];
char passcourse[100][6];
int remainCourse[100] = {0};

int Hours_Attempted = 0;
int Hours_Completed = 0;
int Remaining_Credits = 0;
int i=0; // The index of input course
int j=0; // The index of passcourse
int k=0; // The index of remain course
float sum=0; //The sum of multiply credits by course grade
char ch;
int info_idx = 0;
int char_idx = 0;

// input the courses' name, credit and precourses
void readIn(void)
{
    while( (ch=getchar()) != '|' ) {
        if( info_idx==0 ) {
            name[i][char_idx] = ch;
        }else if ( info_idx==1 ) {
            credit[i] = ch;
        }else {
            precourse[i][char_idx] = ch;
        }
        
        char_idx = char_idx + 1;
    }
    char_idx = 0;
    info_idx = info_idx + 1;
}

// print the message of the courses
void prtCredit(void)
{
    if( Hours_Attempted==0 ) {
        printf("GPA: 0.0\n");
    }else{
        printf("GPA: %.1f\n", sum/Hours_Attempted); //output the GPA
    }
    
    printf("Hours Attempted: %d\n", Hours_Attempted); // output the attempted hours
    printf("Hours Completed: %d\n", Hours_Completed); // output the completed hours
    printf("Credits Remaining: %d\n", Remaining_Credits); //output the remaining credits
    printf("\nPossible Courses to Take Next\n");

}

// find wheteher current course has been passed
// :param cur: the name of current course
// :return: if passed return 1 else return 0
int ifFind(char cur[])
{
    for( int n=0; n<100; n = n + 1 ) { // find in passed course
        if( strcmp(cur, passcourse[n])==0 ) { // if there is one course in the set being studied
            return 1;
        }
    }
    return 0;
}

int main(){
    
    // initialize
    memset(name, '\0', 100 * 6);
    memset(precourse, '\0', 100 * 300);
    memset(grade, '\0', 100);
    memset(passcourse, '\0', 100 * 6);

    // input courses' messages
    while((ch = getchar()) != '\n') {
        // current ch is the 1st ch of CNAME
        name[i][char_idx] = ch;
        char_idx = char_idx + 1;
        // CNAME, CREDIT, PREC
        readIn(); // input CNAME
        readIn(); // input CREDIT
        readIn(); // input PREC
        if((ch = getchar()) != '\n') { // input grade
            grade[i] = ch;
            getchar();
        }
        char_idx = 0;
        info_idx = 0;
                
        if(grade[i] >= 'A' && grade[i]<='D' ) { //  has passed the course
            sum = sum + ('E'-grade[i])*(credit[i]-'0');             // the score is added to total grade
            Hours_Attempted = Hours_Attempted + (credit[i]-'0'); // the credit is added to attempt credit
            Hours_Completed = Hours_Completed + (credit[i]-'0'); // the credit is added to completed credit
            strcpy(passcourse[j], name[i]); // the course is added to passed courses
            j = j + 1; // the index of passed courses plus 1
        }else if( grade[i]=='F' ) { //  not pass the course
            Hours_Attempted = Hours_Attempted + (credit[i]-'0');     // the credit is added to attempt credit
            Remaining_Credits = Remaining_Credits + (credit[i]-'0'); // the credit is added to remaining credit
            remainCourse[k] = i; // the course is added to remain course
            k = k + 1; // the index of remain courses plus 1
        }else { // the course has not been studied
            Remaining_Credits = Remaining_Credits + (credit[i]-'0'); // the credit is added to remaining credit
            remainCourse[k] = i; // the course is added to remain course
            k = k + 1; // the index of remain courses plus 1
        }

        i = i + 1; // the index of input courses plus 1
    }
    
    prtCredit();
    
    if( k==0 ) { //there is no course remained
        printf("  None - Congratulations!\n"); // output no remains tips
    }else {
        for( int m=0; m<k; m = m + 1 ) {
            int canStudy = 1; // assume the course can be studied
            int isSame = 0;  // judge if two courses are same
            int preIndex = 0; // the index of precourse's name
            int isChecked = 0;
            char pre = precourse[remainCourse[m]][preIndex];
            
            char cur[6] = {'\0'}; // current precourse's name
            memset(cur, '\0', 6);
            int cnt = 0;
            while( pre!='\0' ) { // while there exists precourses
                if( pre==',' ) { // ',' means "and"
                    isSame = 0;
                    isChecked = 0;
                    cnt = 0;
                    if( canStudy ) {
                        isSame = ifFind(cur);
                        if( isSame==0 ) { // the precourse is not passed
                            canStudy = 0;
                        }
                    }
                    memset(cur, '\0', 6);
                }else if( pre==';' ) { // ';' means "or"
                    isSame = 0;
                    isChecked = 1;
                    cnt = 0;
                    if( canStudy ) {
                        isSame = ifFind(cur);
                    }
                    if( isSame==0 ) { // the precourse is not passed
                        canStudy = 0;
                    }
                    if( canStudy==0 ) { // the set cannot be studied
                        canStudy = 1;   // enter next set
                    }else {
                        break;
                    }
                    memset(cur, '\0', 6);
                }else {
                    cur[cnt] = pre;
                    cnt = cnt + 1;
                    isChecked = 0;
                }
                preIndex = preIndex + 1;
                pre = precourse[remainCourse[m]][preIndex];
            }

            if( canStudy==0 ) { // if precourses are not taken
                continue;
            }
            else {
                if ( cur[0]=='\0' ) { // if there is no precourse
                    canStudy = 1;
                }
                else { // check the last precourse
                    if( isChecked==0 ) {
                        isSame = 0;
                        isSame = ifFind(cur);
                        if( isSame==0 ) {
                            canStudy = 0;
                        }
                    }
                }
                if( canStudy ) { // if the course can be studied
                    printf("  %s\n",name[remainCourse[m]]); // output the course
                }
            }
        }
    }
    return 0;
}
