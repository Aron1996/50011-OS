const char * usage =
"                                                               \n"
"myhttp-server:                                                 \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   myhttp <port>                                       \n"
"Different Concurrency options [-f|-f|-p] or empty              \n"
"	[Empty] - Runs a basic server with no concurrency           \n"
"	-f - Create a new Process for each request                  \n"
"	-t Create a new Thread for each request                     \n"
"	-p Use a Pool of Threads                                    \n"
"Where 1024 < port < 65536.             \n"
"                                                               \n"
"In another window type:                                       \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where daytime-server  \n"
"is running. <port> is the port number you used when you run   \n"
"daytime-server.                                               \n"
"                                                               \n"
"Then type your file name and return. You will download the file"
"                                                             \n";
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <dirent.h>

int QueueLength = 5;

// Processes time request
void processRequest( int socket );
bool endsWith(const char * str1, const char * str2);
void processRequestThread(int socket);
void poolSlave(int socket);
void sort_dir(char * cwd, char * docPath, DIR* dirp, int case1, int socket);

pthread_mutex_t mt;
pthread_mutexattr_t mattr;

int
main( int argc, char ** argv )
{
    // Get the port from the arguments
    int port ;
    int mode = 0;
    // Print usage if not enough arguments
    if ( argc < 2 ) {
        port = 3396;
        mode = 0;
    }if (argc == 2)
    {
        if (isdigit(*argv[2])) {
            port = atoi( argv[2] );
            mode = 0;
        }
    }
   	if (argc == 3)
   	{
        port = atoi( argv[2] );
        if (argv[1][0] == '-')
        {
            if (argv[1][1] == 'f')
            {
                mode = 1;
            }else if (argv[1][1] == 't')
            {
                mode = 2;
            }else if (argv[1][1] == 'p')
            {
                mode = 3;
            }else{
                printf("it goes else\n");
                fprintf( stderr, "%s", usage );
                exit( -1 );
            }
        }
   	}
   	if (port < 1024 || port > 65535)
   	{
        fprintf( stderr, "%s", usage );
        exit( -1 );
   	}
    
    
    
    // Set the IP address and port for this server
    struct sockaddr_in serverIPAddress;
    memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
    serverIPAddress.sin_family = AF_INET;
    serverIPAddress.sin_addr.s_addr = INADDR_ANY;
    serverIPAddress.sin_port = htons((u_short) port);
    
    // Allocate a socket
    int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
    if ( masterSocket < 0) {
        perror("socket");
        exit( -1 );
    }
    
    // Set socket options to reuse port. Otherwise we will
    // have to wait about 2 minutes before reusing the sae port number
    int optval = 1;
    int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR,
                         (char *) &optval, sizeof( int ) );
    
    // Bind the socket to the IP address and port
    int error = bind( masterSocket,
                     (struct sockaddr *)&serverIPAddress,
                     sizeof(serverIPAddress) );
    if ( error ) {
        perror("bind");
        exit( -1 );
    }
    
    // Put socket in listening mode and set the
    // size of the queue of unprocessed connections
    error = listen( masterSocket, QueueLength);
    if ( error ) {
        perror("listen");
        exit( -1 );
    }
    if (mode == 3) {
        printf("Mode == 3\n");
        pthread_mutexattr_init(&mattr);
        pthread_mutex_init(&mt, &mattr);
        pthread_t tid[QueueLength];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
        
        for (int i = 0; i < QueueLength; i++) {
            pthread_create(&tid[i], &attr, (void * (*)(void *))poolSlave, (void *)masterSocket);
        }
        pthread_join(tid[0], NULL);
    }else{
        while ( 1 ) {
            
            // Accept incoming connections
            struct sockaddr_in clientIPAddress;
            int alen = sizeof( clientIPAddress );
            int slaveSocket = accept( masterSocket,
                                     (struct sockaddr *)&clientIPAddress,
                                     (socklen_t*)&alen);
            if ( slaveSocket < 0 ) {
                perror( "accept" );
                exit( -1 );
            }
            
            if (mode ==0)
            {
                processRequest(slaveSocket);
                close(slaveSocket);
            }else if (mode == 1)
            {
                pid_t slave = fork();
                if (slave == 0)
                {
                    processRequest(slaveSocket);
                    close(slaveSocket);
                    exit(EXIT_SUCCESS);
                }
                close( slaveSocket );
            }else if (mode == 2)
            {
                pthread_t tid;
                pthread_attr_t attr;
                
                pthread_attr_init(&attr);
                pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
                
                pthread_create(&tid, &attr, (void * (*)(void *))processRequestThread, (void *)slaveSocket);
       	    }
        }
        
    }
}
void processRequestThread(int socket) {
    processRequest(socket);
    close(socket);
}
void poolSlave(int socket){
    while(1){
        pthread_mutex_lock(&mt);
        
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);
        int slaveSocket = accept(socket,(struct sockaddr *)&clientIPAddress,(socklen_t*)&alen);
        
        if ( slaveSocket < 0 ) {
            perror( "accept" );
            exit( -1 );
        }
        
        processRequest(slaveSocket);
        close(slaveSocket);
    }
}

void
processRequest( int fd )
{
    // Buffer used to store the name received from the client
    const int MaxName = 1024;
    char currString[ MaxName + 1 ];
    char name[ MaxName + 1 ];
    char * errorMessage;
    int nameLength = 0;
    int n;
    int getCheck = 0;
    char nameDoc[1025];
    int nameDocLength = 0;
    int docDone = 0;
    int docCheck = 0;
    const char * header = "<!DOCTYPE HTML PUBLIC	\"-//IETF//DTD HTML//EN\">\n \
    <head>\n\
    <title>CS50011 lab 7: HTTP Server</title>\n\
    </head>\n\
    <body>\n\
    <h1>CS50011 lab7: HTTP Server</h1>\n\
    <table>\n\
    <tr><th valign=\"top\"><img src=\"/icons/blank.gif\"alt=\"[ICO]\"></th><th><a href=\"?C=N;O=A\">Name</a></th><th><a href=\"?C=M;O=A\">Last modified</a></th><th><a href=\"?C=S;O=A\">Size</a></th><th></tr>\
    <tr><th colspan=\"5\"><hr></th></tr>\n";
    const char * footer = "\
    <tr><th colspan=\"5\"><hr></th></tr>\n\
    <hr>\n \
    </table>\n\
    </body>\n\
    </html>\n\
    ";
    // Currently character read
    unsigned char newChar;
    
    // Last character read
    unsigned char lastChar = 0;
    
    int get = 0;
    
    char docPath[MaxName + 1] = {0};
    
    char contentType[MaxName + 1] = {0};
    
    //
    // The client should send <name><cr><lf>
    // Read the name of the client character by character until a
    // <CR><LF> is found.
    //
    
    while ((n = read(fd, &newChar, sizeof(newChar)))) {
        //printf("%c", newChar);
        name[nameLength++] = newChar;
        
        if (docCheck) {
            //printf("It came in here\n");
            //int counter = 0;
            char character;
            docPath[nameDocLength++] = newChar;
            while ((n = read(fd, &newChar, sizeof(newChar)))) {
                //printf("It went in here: %d\n", counter++);
                if (newChar == ' ') {
                    docDone = 1;
                    docPath[nameDocLength++] = '\0';
                    break;
                }
                name[nameLength++] = newChar;
                docPath[nameDocLength++] = newChar;
                //printf("%c", newChar);
            }
        }
        if (docDone) {
            //printf("nameDoc: %s\n", nameDoc);
            docDone = 0;
            docCheck = 0;
        }
        if (newChar == 'T') {
            if (name[nameLength - 2] == 'E' && name[nameLength - 3] == 'G') {
                //printf("\nGET Found Bitches\n");
                getCheck = 1;
            }
        }
        if (newChar == ' ') {
            if (getCheck) {
                getCheck = 0;
                docCheck = 1;
            }
        }
        
        //Break out only when you hit /r/n/r/n
        if (newChar == '\n') {
            if (name[nameLength - 2] == '\r' && name[nameLength - 3] == '\n'
                && name[nameLength - 4] == '\r') {
                break;
            }
        }
    }
    printf("request = %s\n", name);
    
    // Add null character at the end of the string
    docPath[ nameLength ] = 0;
    char cwd[256] = {0};
    getcwd(cwd, sizeof(cwd));
    
    if(strncmp(docPath, "/icons", strlen("/icons"))==0){
        strcat(cwd, "/http-root-dir");
        strcat(cwd, docPath);
    }else if(strncmp(docPath, "/htdocs", strlen("/htdocs"))==0){
        strcat(cwd, "/http-root-dir");
        strcat(cwd, docPath);
    }else if(strncmp(docPath, "/cgi-bin", strlen("/cgi-bin"))==0){
        strcat(cwd, "/http-root-dir");
        strcat(cwd, docPath);
    }else{
        if (strcmp(docPath, "/")==0)
        {
            strcpy(docPath, "index.html");
        }
        strcat(cwd, "/http-root-dir/htdocs/");
        strcat(cwd, docPath);
    }
    printf("docPath = %s\n", docPath);
    if (strstr(docPath, ".."))
    {
        char resolve[MaxName]={0};
        realpath(docPath, resolve);
        printf("realpath%s\n", resolve);
        if (strlen(resolve) >= strlen(cwd)+strlen("/http-root-dir"))
        {
            strcpy(cwd, resolve);
        }else{
            perror("realPath");
            errorMessage = "Invalid path";
            write(fd, "HTTP/1.1 404 File Not Found", 27);
            write(fd, "\r\n", 2);
            write(fd, "Server: CS 50011 lab7", 21);
            write(fd, "\r\n", 2);
            write(fd, "Content-type: text/html", 23);
            write(fd, "\r\n", 2);
            write(fd, "\r\n", 2);
            
            write(fd, errorMessage, strlen(errorMessage));
        }
    }
    int case1 =-1;
    if(strstr(cwd, "?C=N;O=A") != 0){
        case1 = 0;                  // name in ascending
    }else if(strstr(cwd, "?C=N;O=D") != 0){
        case1 = 1;                  // name in descending
    }else if(strstr(cwd, "?C=M;O=A") != 0){
        case1 = 2;                  // time in ascending
    }else if(strstr(cwd, "?C=M;O=D") != 0){
        case1 = 3;                  // tine in descending
    }else if(strstr(cwd, "?C=S;O=A") != 0){
        case1 = 4;                  // size in ascending
    }else if(strstr(cwd, "?C=S;O=D") != 0){
        case1 = 5;                  // size in descending
    }
    printf("endcwd = %s\n", cwd);
    printf("case=%d\n", case1);
    
    if(endsWith(docPath, ".html") || endsWith(docPath, ".html/")){
        strcpy(contentType, "text/html");
    }
    if(endsWith(docPath, ".image") || endsWith(docPath, ".image/") ||
       endsWith(docPath, ".gif") || endsWith(docPath, ".gif/")){
        strcpy(contentType, "image/gif");
    }
    else{
        strcpy(contentType, "text/plain");
    }
    
    struct stat s;
    if (case1 != -1) {
        cwd[strlen(cwd)-8] = '\0';
        printf("final cwd:%s", cwd);
        write(fd, "HTTP/1.1 200 ", 13);
        write(fd, docPath, strlen(docPath));
        write(fd, "\r\n", 2);
        write(fd, "Server: CS 50011 lab7", 21);
        write(fd, "\r\n", 2);
        write(fd, "Content-Type ", 14);
        write(fd, contentType, strlen(contentType));
        write(fd, "\r\n", 2);
        write(fd, "\r\n", 2);
        write(fd, header, strlen(header));
        DIR* dirp = opendir(cwd);
        struct dirent * dp;
        sort_dir(cwd, docPath, dirp, case1, fd);
        closedir(dirp);
        write(fd, footer, strlen(footer));
        
    }else{
        if( stat(cwd,&s) == 0 )
        {
            if( s.st_mode & S_IFDIR )
            {
            
                write(fd, "HTTP/1.1 200 ", 13);
                write(fd, docPath, strlen(docPath));
                write(fd, "\r\n", 2);
                write(fd, "Server: CS 50011 lab7", 21);
                write(fd, "\r\n", 2);
                write(fd, "Content-Type ", 14);
                write(fd, contentType, strlen(contentType));
                write(fd, "\r\n", 2);
                write(fd, "\r\n", 2);
                write(fd, header, strlen(header));
                DIR* dirp = opendir(cwd);
                struct dirent * dp;
                printf("it gose here?\n");
                sort_dir(cwd, docPath, dirp, 0, fd);
                closedir(dirp);

                write(fd, footer, strlen(footer));
            }
            else if( s.st_mode & S_IFREG )
            {
                //it's a file
                FILE* fp;
                if (strstr(contentType, "image"))
                {
                    fp = fopen (cwd, "rb");
                }else{
                    fp = fopen (cwd, "r");
                }
                if (fp > 0)
                {
                    write(fd, "HTTP/1.1 200 ", 13);
                    write(fd, docPath, strlen(docPath));
                    write(fd, "\r\n", 2);
                    write(fd, "Server: CS 50011 lab7", 21);
                    write(fd, "\r\n", 2);
                    write(fd, "Content-Type ", 14);
                    write(fd, contentType, strlen(contentType));
                    write(fd, "\r\n", 2);
                    write(fd, "\r\n", 2);
                    
                    long count = 0;
                    
                    char c;
                    
                    while (count = read(fileno(fp), &c, sizeof(c))) {
                        if (write(fd, &c, sizeof(c)) != count) {
                            perror("write");
                            printf("it goes here\n");
                            errorMessage = "Invalid path";
                            write(fd, "HTTP/1.1 404 File Not Found", 27);
                            write(fd, "\r\n", 2);
                            write(fd, "Server: CS 50011 lab7", 21);
                            write(fd, "\r\n", 2);
                            write(fd, "Content-type: text/html", 23);
                            write(fd, "\r\n", 2);
                            write(fd, "\r\n", 2);
                            
                            write(fd, errorMessage, strlen(errorMessage));
                            break;
                        }
                    }
                    
                    fclose(fp);
                    
                }
            }
            else
            {
                const char *message = "File not found";
                
                write(fd, "HTTP/1.1 404 File Not Found", 27);
                write(fd, "\r\n", 2);
                write(fd, "Server: CS 50011 lab7", 21);
                write(fd, "\r\n", 2);
                write(fd, "Content-type: text/html", 23);
                write(fd, "\r\n", 2);
                write(fd, "\r\n", 2);
                
                write(fd, message, strlen(message));
            }
        }
        else
        {
            //error
            const char *message = "File not found";
            
            write(fd, "HTTP/1.1 404 File Not Found", 27);
            write(fd, "\r\n", 2);
            write(fd, "Server: CS 50011 lab7", 21);
            write(fd, "\r\n", 2);
            write(fd, "Content-type: text/html", 23);
            write(fd, "\r\n", 2);
            write(fd, "\r\n", 2);
            
            write(fd, message, strlen(message));
        }
        
    }
}
bool endsWith(const char * str1, const char * str2) {
    
    if( str1 == NULL || str2 == NULL )
        return false;
    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);
    if(str2_len > str1_len)
        return false;
    
    return 0 == strncmp( str1 + str1_len - str2_len, str2, str2_len );
}
void sort_dir(char * cwd, char * docPath, DIR* dirp, int case1, int socket){

    int dir_length=0;
    struct dirent * dp;
    struct dirent * namelist[128];
    long mod_time[128];
    long file_size[128];
    long temp_time, temp_size;
    int isFile[128];
    int k=0, temp_int;
    struct stat s;
    struct dirent * filename;
    while ((dp = readdir(dirp)) != NULL) {
        dir_length++;
        namelist[k] = dp;

        char file_path[200];
        bzero(file_path,200);
        strcat(file_path,cwd);
        strcat(file_path,"/");
        strcat(file_path,dp->d_name);
        
        stat(file_path,&s);
        mod_time[k] = s.st_mtime;
        if (S_ISDIR(s.st_mode)) {
            file_size[k] = -1;
            if (namelist[k]->d_name[0] != '.' ) {
                isFile[k] =1;          // it's not file
            }
        }
        else if( s.st_mode & S_IFREG ) {
            file_size[k] = s.st_size;
            if (namelist[k]->d_name[0] != '.' ) {
                isFile[k] =2;          /// it's file
            }
        }else{
            isFile[k] =3;          // dummy
        }
        k++;
    }
    if ( case1 == 0 ) { //Sort By Name.
        for(int i = 0; i < dir_length; i ++){
            for(int j = 0 ;j < dir_length-i-1; j ++){
                struct dirent * filename;
                if (strcmp(namelist[j]->d_name, namelist[j+1]->d_name) > 0) {
                    temp_time = mod_time[j+1];
                    mod_time[j+1] = mod_time[j];
                    mod_time[j] = temp_time;
                    temp_size = file_size[j+1];
                    file_size[j+1] = file_size[j];
                    file_size[j] = temp_size;
                    filename = namelist[j+1];
                    namelist[j+1] = namelist[j];
                    namelist[j] = filename;
                    temp_int = isFile[j+1];
                    isFile[j+1]=isFile[j];
                    isFile[j]=temp_int;
                }
            }
        }
    }else if (case1 == 2 ) { // Sort By Modify Time.
        for(int i = 0; i < dir_length; i ++){
            for(int j = 0 ;j < dir_length-i-1; j ++){
                if (mod_time[j] > mod_time[j+1]) {
                    temp_time = mod_time[j+1];
                    mod_time[j+1] = mod_time[j];
                    mod_time[j] = temp_time;
                    temp_size = file_size[j+1];
                    file_size[j+1] = file_size[j];
                    file_size[j] = temp_size;
                    filename = namelist[j+1];
                    namelist[j+1] = namelist[j];
                    namelist[j] = filename;
                    temp_int = isFile[j+1];
                    isFile[j+1]=isFile[j];
                    isFile[j]=temp_int;
                }
            }
        }
    }else if ( case1 == 4 ) { //Sort By Size.
        for(int i = 0; i < dir_length; i ++){
            for(int j = 0 ;j < dir_length-i-1; j ++){
                if (file_size[j] > file_size[j+1]) {
                    temp_time = mod_time[j+1];
                    mod_time[j+1] = mod_time[j];
                    mod_time[j] = temp_time;
                    temp_size = file_size[j+1];
                    file_size[j+1] = file_size[j];
                    file_size[j] = temp_size;
                    filename = namelist[j+1];
                    namelist[j+1] = namelist[j];
                    namelist[j] = filename;
                    temp_int = isFile[j+1];
                    isFile[j+1]=isFile[j];
                    isFile[j]=temp_int;
                }
            }
        }	
    }
    printf("dir length:%d\n", dir_length);
    for(int i = 0; i < dir_length; i ++){
        printf("filename:%s\n",namelist[i]->d_name);
    }
    for(int i = 0; i < dir_length; i ++){
        printf("file time:%ld\n",mod_time[i]);
    }
    for(int i = 0; i < dir_length; i ++){
        printf("filelength:%ld\n",file_size[i]);
    }
    const char * str = "<tr><td valign=\"top\"><img src=\"/icons/back.gif\" alt=\"[PARENTDIR]\"></td><td><a href=\"";
	write(socket, str, strlen(str));
	char * temp = docPath;
    char * temp2 = cwd;
    printf("temp = %s\n", temp);
    printf("temp2 = %s\n", temp2);
    int i = strlen(temp)-2;
    while (i>=0){
        if (temp[i]=='/' && temp[i+1] == '/') {
            while(temp[i+1] != '\0') {
                temp[i] = temp[i+1];
                i++;
            }
            temp[i] = '\0';
            break;
        }
        i--;
    }
    i = strlen(temp)-1;
    while (i >= 0) {
        if (temp[i] == '/' && i != strlen(temp)-1) {
            temp[i+1] = '\0';
            break;
        }
        i--;
    }
    i = strlen(temp2)-2;
    while (i >= 0) {
        if (temp2[i] == '/' && i != strlen(temp2)-1) {
            temp2[i+1] = '\0';
            break;
        }
        i--;
    }
    i = strlen(temp2)-1;
    while (i >= 0) {
        if (temp2[i] == '/' && i != strlen(temp2)-1) {
            temp2[i+1] = '\0';
            break;
        }
        i--;
    }
    char cwd2[256] = {0};
    getcwd(cwd2, sizeof(cwd2));
    if (strlen(temp2) <= strlen(cwd2)+strlen("/http-root-dir")) {
        temp = "/";
    }
    printf("temp = %s\n", temp);
    printf("temp2 = %s\n", temp2);
    printf("cwd2 = %s\n", cwd2);

    write(socket, temp, strlen(temp));
    write(socket, "\">Parent Directory</a></td></tr>", strlen("\">Parent Directory</a></td></tr>"));
    
    for(int i = 0; i < dir_length; i ++){
        
        if (isFile[i] == 1) {
            const char * str = "<tr><td valign=\"top\">\
            <img src=\"/icons/folder.gif\" alt=\"[DIR]\"></td><td>\
            <A HREF=\"";
            write(socket, str, strlen(str));
            write(socket, namelist[i]->d_name, strlen(namelist[i]->d_name));
            const char * str2 = "\">";
            write(socket, str2, strlen(str2));
            write(socket, namelist[i]->d_name, strlen(namelist[i]->d_name));
            const char * str3 = "/</A></td><td align=\"right\">";
            const char * str4 = "</td><td align=\"right\">";
            const char * str5 = " - </td></tr>\n";
            char time[256];
            write(socket, str3, strlen(str3));
            struct tm *p;
            p = gmtime(&(mod_time[i]));
            sprintf(time, "%d-%d-%d %d:%d:%d", p->tm_year+1900, p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
            write(socket, time, strlen(time));
            write(socket, str4, strlen(str4));
            write(socket, str5, strlen(str5));

        }else if (isFile[i] ==2){
            const char * str = "<tr><td valign=\"top\"><img src=\"/icons/blank.gif\" alt=\"[]\"></td>\
            <td><A HREF=\"";
            write(socket, str, strlen(str));
            write(socket, namelist[i]->d_name, strlen(namelist[i]->d_name));
            const char * str2 = "\">";
            write(socket, str2, strlen(str2));
            write(socket, namelist[i]->d_name, strlen(namelist[i]->d_name));
            const char * str3 = "</A></td><td align=\"right\">";
            const char * str4 = "</td><td align=\"right\">";
            const char * str5 = "</td></tr>\n";
            char size[256];
            char time[256];
            struct tm *p;
            p = gmtime(&(mod_time[i]));
            sprintf(time, "%d-%d-%d %d:%d:%d", p->tm_year+1900, p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
            if(file_size[i] > 1024) {
                file_size[i] = file_size[i]/1024;
                if(file_size[i] > 1024) {
                    file_size[i] = file_size[i]/1024;
                    sprintf(size, "%ldM", file_size[i]);
                }
                else{
                    sprintf(size, "%ldK", file_size[i]);
                }
            }
            else{
                sprintf(size, "%ld", file_size[i]);
            }
            write(socket, str3, strlen(str3));
            write(socket, time, strlen(time));
            write(socket, str4, strlen(str4));
            write(socket, size, strlen(size));	
            write(socket, str5, strlen(str5));
        }
    }
    
}





