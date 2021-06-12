# soal-shift-sisop-modul-4-F05-2021\

## soal1
### soal 1A
Yang diminta adalah ketika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
dapat dilakukan dengan FUSE sebagaimana pada umumnya, namun pada fungsi `xmp_readdir`
```
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){ //baca directory
	char * strToEnc1 = strstr(path, prefix);
    
	if(strToEnc1 != NULL) {
        decode1(strToEnc1);
    }

	char newPath[1000];
	if(strcmp(path,"/") == 0){
		path=directoryPath;
		sprintf(newPath, "%s", path);
	} else 
        sprintf(newPath, "%s%s", directoryPath, path);

	int result = 0;
	struct dirent *dir;
	DIR *dp;
	(void) fi;
	(void) offset;
	dp = opendir(newPath);
	if (dp == NULL) return -errno;

	while ((dir = readdir(dp)) != NULL) { //buat loop yang ada di dalam directory
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = dir->d_ino;
		st.st_mode = dir->d_type << 12;
		if(strToEnc1 != NULL){
			encode1(dir->d_name); //encode yang ada di dalam directory sekarang
        }
		
		result = (filler(buf, dir->d_name, &st, 0));
		if(result!=0) break;
	}

	closedir(dp);
	return 0;
}
```
Untuk Fungsi `encode1` nya adalah sebagai berikut
```
void encode1(char* strEnc1) { 
	if(strcmp(strEnc1, ".") == 0 || strcmp(strEnc1, "..") == 0)
        return;
    
    int strLength = strlen(strEnc1);
    for(int i = 0; i < strLength; i++) {
		if(strEnc1[i] == '/') 
            continue;
		if(strEnc1[i]=='.')
            break;
        
		if(strEnc1[i]>='A'&&strEnc1[i]<='Z')
            strEnc1[i] = 'Z'+'A'-strEnc1[i];
        if(strEnc1[i]>='a'&&strEnc1[i]<='z')
            strEnc1[i] = 'z'+'a'-strEnc1[i];
    }
}
```

### soal 1b
Ketika direname di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
Untuk Fungsi `decode1` nya adalah sebagai berikut
```
void decode1(char * strDec1){ //decrypt encv1_
	if(strcmp(strDec1, ".") == 0 || strcmp(strDec1, "..") == 0 || strstr(strDec1, "/") == NULL) 
        return;
    
    int strLength = strlen(strDec1), s=0;
	for(int i = strLength; i >= 0; i--){
		if(strDec1[i]=='/')break;

		if(strDec1[i]=='.'){//nyari titik terakhir
			strLength = i;
			break;
		}
	}
	for(int i = 0; i < strLength; i++){
		if(strDec1[i]== '/'){
			s = i+1;
			break;
		}
	}
    for(int i = s; i < strLength; i++) {
		if(strDec1[i] =='/'){
            continue;
        }
        if(strDec1[i]>='A'&&strDec1[i]<='Z'){
            strDec1[i] = 'Z'+'A'-strDec1[i];
        }
        if(strDec1[i]>='a'&&strDec1[i]<='z'){
            strDec1[i] = 'z'+'a'-strDec1[i];
        }
    }
	
}
```
### soal 1c
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode. Untuk soal ini telah terimplementasi pada fungsi `readdir` pada soal 1a dan soal 1b.

### soal 1d
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori] .
berikut adalah fungsi untuk penulisan log.
```
void logging2(const char* old, char* new) {
	// char* filename = basename(new);

	FILE * logFile = fopen("/home/juned/fs.log", "a");
    fprintf(logFile, "%s → %s\n", old, new);
    fclose(logFile);
}
```
### soal 1e
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya. (rekursif).
Untuk soal ini telah terimplementasi di soal 1a dan soal 1b.

## soal2
Kesulitan :
- Belum paham bagaimana caranya untuk membedakan enkripsi yang digunakan dari folder awalan "RX_", antara folder hasil mkdir, dan folder hasil rename.<br> 
## soal4
### Soal 4A
Soal ini diminta untuk membuat sebuah Log system yang bernama `SinSeiFS.log` pada direktori home user
yang dimana Log system ini digunakan untuk menyimpan daftar perintah system call yang telah dijalankan pada filesystem.<br><br>

Disini sudah terbuat fungsi yang bernama `logging` untuk membuat sebuah Log system. Berikut bentuk fungsinya:

```
void logging(char* c, int type){
    FILE * logFile = fopen("/home/juned/SinSeiFS.log", "a");
```

`FILE * logFile = fopen("/home/juned/SinSeiFS.log", "a");` ini digunakan untuk membuat file Log system yang dimana akan terbentuk 
dan akan disimpan pada direktori home user. Didalam file Log system, terdapat deskripsi info yang berupa waktu yang menandakan waktu 
jalannya pada suatu file system.<br><br>

Berikut bentuk isi deskripsi infonya:

```
time_t currTime;
	struct tm * time_info;
	time ( &currTime );
	time_info = localtime (&currTime);
```

untuk `time ( &currTime )` ini disesuaikan dengan waktu secara "Real Time" atau menggunakan "Local Time". Dan untuk penggunaan `struct` pada `struct tm * time_info` ini digunakan supaya waktu yang ditampilkan secara terurut.

### Soal 4B, 4C, 4D, 4E
Dikarenakan Sin dan Sei ini suka dengan kerapian, maka untuk soal 4B, 4C, 4D, 4E diminta untuk membagi log yang telah dibuat menjadi 2 bagian log level,
yaitu `INFO` dan `WARNING`. Dari 2 log level ini, bagian `WARNING` digunakan untuk mencatat syscall `rmdir` dan `unlink`, sedangkan untuk bagian `INFO` 
digunakan untuk mencatat syscall sisanya atau selain dari syscall `rmdir` dan juga `unlink`.<br><br> 

Format untuk loggingnya sendiri sebagai berikut :
`[Level]:[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`

berikut bentuk funsgi kodingannya:

```
int yr=time_info->tm_year + 1900;
	int mon=time_info->tm_mon + 1;
	int day=time_info->tm_mday;
	int hour=time_info->tm_hour;
	int min=time_info->tm_min;
	int sec=time_info->tm_sec;

    if(type==1){//info
        fprintf(logFile, "INFO::%02d%02d%d-%d:%d:%d:%s\n", day, mon, yr, hour, min, sec, c);
    }
    else if(type==2){ //warning
        fprintf(logFile, "WARNING::%02d%02d%d-%d:%d:%d:%s\n", day, mon, yr, hour, min, sec, c);
    }
    fclose(logFile);
}
```

Untuk `int yr`, `int mon`, `int day`, `int hour`, `int min`, dan `int sec` digunakan untuk menampilkan waktu berjalannya suatu log pada file system yang
sesuai dengan format yang telah ditentuan. Setelah itu dalam proses terbuatnya log level `INFO` dan juga `WARNING` ini, pertama-pertama membuat sebuah 2 fungsi 
percabangan "IF". Saat pengecekan jika bertipe 1, maka logFile akan mencetak sebuah log level yang bernama `INFO` beserta dengan isiannya, yaitu waktu `(time_info)` 
dan juga beberapa syscall yang sudah dijalankan pada file system, seperti `getattr`, `readdir`, `read`, `mkdir`, `mknod`, `rename`, `open`, dan `write`. Lalu, 
dilakukan pengecekan lagi jika bertipe 2, maka logFile akan mencetak sebuah log level yang bernama `WARNING` beserta dengan isiannya seperti pada log level `INFO`, 
yaitu `(time_info)` dan juga beberapa syscall sisanya, seperti `rmdir` dan juga `unink`.<br><br>

Berikut ilustrasi jika program dijalankan untuk soal 4a - 4e:
