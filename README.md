# soal-shift-sisop-modul-4-F05-2021
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
