# FP_SISOP20_F08
Final Project Sistem Operasi 2020
#
1. Sheinna Yendri (05111840000038)
2. Muhammad Rafi Yudhistira (05111840000115)
#
Lima Command yang kami pilih:
1. [date](#date)
2. [tree](#tree)
3. [cmp](#cmp)
4. [tail](#tail)
5. [base64](#base64)
#
Agar kelima command dapat dicompile dan dirun pada QEMU, maka harus dilakukan edit pada file ```Makefile``` pada 2 bagian:
1. UPROGS, ditambahkan nama command yang dibuat dalam format ```_command\```.
2. EXTRA, ditambahkan file .c yang memuat implementasi command tersebut dalam format ```command.c```.
#

## date
Command date meminta kami menampilkan date atau time dengan beberapa kriteria yaitu:
1. date
2. date + "%d-%m-%y"
3. date -s
4. date -r

Di mana command pertama untuk menampilkan date keseluruhan (default), kemudian command kedua untuk menampilkan date dengan format tertentu, date -s untuk memodifikasi date, dan date -r untuk menampilkan last modification time untuk file yang dipassing dalam argumen.

Untuk mengimplementasikan fungsi date, kami harus memodifikasi 5 file terlebih dahulu (agar terdapat system yang dapat memanggil fungsi date). Kelima file tersebut adalah:
1. syscall.h
- menambahkan ```#define SYS_date 22```

2. syscall.c
- menambahkan ```[SYS_date] sys_date,``` pada fungsi ```static int (*syscalls[ ])(void).```

3. sysprog.c
- menambahkan:
```c
int sys_date(void)
    {
      struct rtcdate *r;
      if(argptr(0, (void*)&r, sizeof(r)) < 0)
       return -1;
      cmostime(r);
      return 0;
     }
```

4. usys.S
- menambahkan ```SYSCALL(date)```

5. user.h
- menambahkan ```int date(struct rtcdate *);```

Untuk implementasi command date, harus secara manual melakukan *parsing* dari setiap mode yang diinginkan user, seperti ```%d, %m, %y```, serta melakukan perhitungan manual untuk hari bulan tanggal serta konversinya ke versi huruf atau angka. Di sini juga dibutuhkan fungsi untuk menentukan tahun kabisat atau tidak untuk menentukan pada bulan Februari 28 atau 29 hari, kemudian pada tahun tersebut secara total 365 atau 366 hari.

Berikut implementasi command date, beserta fungsi-fungsinya:
```c
int main(int argc, char *argv[])
{
  if (argc == 1)
    today();
  else if (argc == 2)
  {
    if ((strcmp("-u", *(argv + 1)) == 0))
      utc_day();
    else
      printf(1, "Invalid command. Please try again.\n");
  }
  
  else
  {
    if (argc == 3)
    {
      if ((strcmp("-d", *(argv + 1)) == 0))
        day(*(argv + 2)); 
      else if (argv[2][0] == '"' && argv[2][1] == '%' && argv[2][strlen(argv[2])-1] == '"')
      {
        spesial_format(argv[2]);
        printf(1, "\n");
      }
      else if((strcmp("-r", *(argv + 1)) == 0)) {
	printf(1, "Last modification time of file.\n");
      }
      else
        printf(1, "Invalid command. Please try again.\n");
    }
    else
      printf(1, "Invalid command. Please try again.\n");
  }
  exit();
}

void spesial_format(char argv[])
{
  struct rtcdate r;
  if (date(&r))
  {
    printf(2, "date failed\n");
    exit();
  }
  // printf(1, " %d/%d/%d\n", r.day, r.month, r.year);
  // printf(1, " %d:%d:%d", r.hour, r.minute, r.second);
  // printf(1, "panjang %d \n%s\n", strlen(argv), argv);
  for (int i = 1; i < strlen(argv)-1; i++)
  {
    if (argv[i] == '%')
    {
      if (argv[i+1] == 'D')
      {
        printf(1, "%d/%d/%d", r.month, r.day, r.year);

      }
      else if (argv[i+1] == 'd')
      {
        printf(1, "%d", r.day);

      }
      else if (argv[i+1] == 'm')
      {
        printf(1, "%d", r.month);

      }
      else if (argv[i+1] == 'y')
      {
        printf(1, "%d", r.year);

      }
      else if (argv[i+1] == 't')
      {
        time();

      }
      else
      {
        printf(1, "invalid command\n");
        exit();
      }
      i++;
    }
    else if(argv[i] == '-')
    {
      printf (1, "-");
    }

  }
  
}

long long power(int x, int y)
{
  long long res = 1;
  for (int i = 0; i < y; i++)
  {
    res = res * x;
  }
  return (res);
}

int check_leap(int x)
{
  int flag = 0;
  if (x % 400 == 0)
    flag = 1;
  else if (x % 100 == 0)
    flag = 0;
  else if (x % 4 == 0)
    flag = 1;
  else
    flag = 0;
  return (flag);
}

void month_name(int x)
{
  switch (x)
  {
  case 1:
    printf(1, " Jan");
    break;
  case 2:
    printf(1, " Feb");
    break;
  case 3:
    printf(1, " Mar");
    break;
  case 4:
    printf(1, " Apr");
    break;
  case 5:
    printf(1, " May");
    break;
  case 6:
    printf(1, " Jun");
    break;
  case 7:
    printf(1, " Jul");
    break;
  case 8:
    printf(1, " Aug");
    break;
  case 9:
    printf(1, " Sep");
    break;
  case 10:
    printf(1, " Oct");
    break;
  case 11:
    printf(1, " Nov");
    break;
  case 12:
    printf(1, " Dec");
    break;
  }
}

void day_name(int x, int y, int z)
{
  int initial_day = 4;
  int count = 0;
  if (x > 1970)
  {
    for (int i = 1970; i < x; i++)
    {
      if (check_leap(i))
        count += 366;
      else
        count += 365;
    }
  }
  for (int i = 1; i < y; i++)
  {
    if (i == 2)
    {
      if (check_leap(x))
        count += 29;
      else
        count += 28;
    }
    else if ((i < 8) && (i % 2 == 1))
      count += 31;
    else if ((i < 8) && (i % 2 == 0))
      count += 30;
    else if ((i >= 8) && (i % 2 == 0))
      count += 31;
    else
      count += 30;
  }
  int final_day = (initial_day + count + z - 1) % 7;
  switch (final_day)
  {
  case 0:
    printf(1, "Sun");
    break;
  case 1:
    printf(1, "Mon");
    break;
  case 2:
    printf(1, "Tue");
    break;
  case 3:
    printf(1, "Wed");
    break;
  case 4:
    printf(1, "Thur");
    break;
  case 5:
    printf(1, "Fri");
    break;
  case 6:
    printf(1, "Sat");
    break;
  }
}

void time()
{
  struct rtcdate r;
  if (date(&r))
  {
    printf(2, "date failed\n");
    exit();
  }
  if (r.minute > 59)
  {
    r.hour += 8;
    r.minute = r.minute - 59;
  }
  else
  {
    r.hour += 7;
  }
  if (r.hour >= 24)
    r.hour -= 24;
  printf(1, " %d:%d:%d", r.hour, r.minute, r.second);
}
```
Contoh saat command date dijalankan:

![date](https://user-images.githubusercontent.com/48936125/81392456-ea222300-9148-11ea-9623-fb13a15a1453.jpg)
#

## tree
Command tree meminta kami untuk menampilkan direktori/file dalam struktur seperti "tree". Terdapat beberapa argumen yang dapat menampilkan tree dari suatu direktori dengan kriteria berbeda:
1. tree
2. tree /path_to_dir
3. tree -d
4. tree -L 3

Untuk command tree yang pertama (default), untuk menampilkan list direktori dan file pada *current working directory* di mana dapat dilambangkan dengan ```.```.

Untuk command tree yang kedua, untuk menampilkan list direktori dan file yang berada **di dalam** direktori yang menjadi argumen tersebut.

Untuk command tree yang ketiga, untuk menampilkan list **direktori saja** pada *current working directory* di mana dapat dilambangkan dengan ```.```.

Untuk command tree yang keempat, untuk menampilkan list direktori dan file **pada kedalaman tertentu** di mana kedalaman yang diinginkan menyesuaikan pada argumen ketiga (harus berupa angka), dalam contoh ini 3.

Dalam mengimplementasikan command tree ini, untuk mempermudah pengerjaan, maka kami membuat suatu fungsi tree, yang memiliki parameter direktori yang ingin ditampilkan pathnya, kedalaman yang diinginkan, level direktori tersebut, serta flag penanda apakah yang dicetak direktori dan file ataukah hanya direktori saja (untuk menangani command tree ketiga). Berikut implementasi fungsi tree yang kami buat:
```c
void tree(char *path, int depth, int indent, int flag)
{
    if (depth == level)
        return;

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }
    int k, id;
    if (!flag && st.type == 2) {
	k = 0;
	char pjg[100];
	id = indent * 4;
	while(k <= id) pjg[k++] = ' ';
	pjg[k++] = '|'; pjg[k++] = '-'; pjg[k++] = '-'; pjg[k] = ' ';
	printf(1, "%s%s\n", pjg, fmtname(path));
	nfile++;
    }
    else if (st.type == 1 && strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
	printf(1, "tree: path too long\n");
    }
    else if(st.type == 1)
    {
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';
	while (read(fd, &de, sizeof(de)) == sizeof(de))
	{
	    if (de.inum == 0) continue;
 	    memmove(p, de.name, DIRSIZ);
	    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
	    p[DIRSIZ] = 0;
	    if (stat(buf, &st) < 0)
	    {
	        printf(1, "tree: cannot stat %s\n", buf);
	        continue;
	    }
	    else if (st.type == 1) //directory
	    {
	        int kk; kk = 0;
 	        char pjg2[100];
	        id = indent * 4;
	        while(kk <= id) pjg2[kk++] = ' ';
	        pjg2[kk++] = '|'; pjg2[kk++] = '-'; pjg2[kk++] = '-'; pjg2[kk] = ' ';
	        printf(1, "%s%s\n", pjg2, fmtname(buf));
	        ndir++;
	    }
	    else if (st.type == 2 && !flag) //file
	    {
		k = 0;
		char pjg[100];
		id = indent * 4;
		while(k <= id) pjg[k++] = ' ';
		pjg[k++] = '|'; pjg[k++] = '-'; pjg[k++] = '-'; pjg[k] = ' ';
		printf(1, "%s%s\n", pjg, fmtname(buf));
		nfile++;
	    }
	    if (st.type == 1) tree(buf, depth + 1, indent + 1, flag);
	}
    }
    close(fd);
}
```

Fungsi void tree tersebut yang nantinya akan menampilkan list direktori dan file pada direktori yang dipassing dan dapat bekerja secara rekursif, karena jika tipe berkas yang ditemui bertipe 1 (T_DIR = direktori), maka akan dilakukan pemanggilan terhadap fungsi void tree lagi dengan parameter path baru, dengan kedalaman dan level yang ditambah 1, serta dengan flag yang tetap menyesuaikan induknya.

Jika kedalaman sudah melebihi batas kedalaman file yang diinginkan, maka akan dihentikan (return).

Selain menggunakan fungsi void tree, kami juga menggunakan 2 fungsi lain untuk mempermudah pengerjaan command, yaitu fungsi untuk mengambil nama folder/file saja dari *full path* yang di-*passing*-kan, serta fungsi kedua adalah untuk pengecekan apabila suatu string yang di-*passing*-kan merupakan integer atau tidak. Berikut implementasi kedua fungsi tersebut:
```c
char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

int isNumber(char *s)
{
    int i;
    for(i = 0; i < strlen(s); i++)
	if(s[i] < 48 || s[i] > 57) return 0;
    return 1;
}
```

Kemudian, untuk menghitung banyaknya file dan direktori, maka setiap akan dicetak file/direktori yang ditemukan, maka variabel bertipa global ```nfile``` dan ```ndir``` akan di-*increment* nilainya, agar terakhir dapat ditampilkan banyaknya.

Untuk menampilkan agar struktur tree lebih mirip command tree sebenarnya pada linux, maka kami menggunakan ```|-- ``` untuk menandakan setiap ada file/direktori yang ingin ditampilkan, dikarenakan indentasi itu mengambil 4 spasi, maka untuk tiap level kedalaman, nanti akan diberikan spasi sebanyak kedalaman * 4, agar indentasi sesuai dan terlihat rapi.

Berikut implementasi main, di mana hanya dilakukan pemanggilan terhadap fungsi tree, sesuai argumen yang diinputkan user:
```c
int main(int argc, char *argv[])
{
    if (argc >= 5) {
	printf(1, "tree: invalid argument\n");
	exit();
    }
    if (argc < 2)
    {
        tree(".", MAX_LEVEL, 0, 0);
    }
    else if (strcmp(argv[1],"-L") == 0 )
    {
	if(!isNumber(argv[2])) {
	    printf(1, "tree: invalid argument\n");
	    exit();
	}
        level = atoi(argv[2]);
        tree(".", 0, 0, 0);
    }
    else if (strcmp(argv[1],"-d") == 0 )
    {
        tree(".", MAX_LEVEL, 0, 1);
    }
    else if (argc == 2)
    {
        tree(argv[1], MAX_LEVEL, 0, 1);
    }
    printf(1, "\n%d directories, %d files\n", ndir, nfile);
    exit();
}
```
Contoh saat command tree dijalankan:

![tree](https://user-images.githubusercontent.com/48936125/81302806-066c8400-90a5-11ea-87a6-5e003d0a3f08.jpg)
#

## cmp
Command cmp meminta kami untuk membandingkan 2 file byte per byte, kemudian menunjukkan perbedaan yang ditemui pertama kali dari kedua file tersebut pada byte ke berapa dan baris ke berapa.
```
Contoh: cmp file1.txt file2.txt
```
Maka akan ditampilkan perbedaan kedua file terletak pada byte ke berapa dan baris (*line*) ke berapa. Apabila ternyata kedua file identik (tidak ada perbedaan sama sekali), maka akan dioutputkan tanda ```_```.

Untuk membuat command ini, kami menggunakan buffer untuk membaca isi dari kedua file, yang kemudian diloop untuk dicek apakah byte per byte dari masing-masing file memiliki perbedaan. Jika iya, maka langsung outputkan index looping yang menandakan byte, serta line ke berapa dapat ditampilkan melalui suatu variabel ```lines``` yang menampung banyaknya baris dari awal pengecekan file. 

Cara menghitung banyaknya baris adalah ketika dilakukan looping untuk membandingkan kedua file, maka akan dicek jika karakter yang ditemui adalah ```\n```, maka variabel ```lines``` akan di-*increment* ```lines++```.

Pada main, akan dilakukan pengecekan argumen seperti biasa, jika banyak argumen yang dipassing <= 2, maka akan dicetak bahwa argumen invalid. Jika tidak, maka akan dicek kedua file yang dipassing seperti yang telah dijelaskan di atas.

Berikut  implementasi cmp:
```c
int main (int argc, char *argv[]) {
  int fd, fd2, i, n;
  if(argc <= 2) {
    printf(1, "cmp: need 2 arguments!");
    exit();
  }
  
  if((fd = open(argv[1], O_RDWR)) < 0) {
    printf(1, "cmp: cannot open %s for reading\n", argv[1]);
    exit();
  }
  char *buffer;
  buffer = (char*) malloc(1000000 * sizeof(char));
  int buffSize = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0)
  {
    for(i = 0; i < n; i++)
    {
      buffer[buffSize] = (char)buf[i];
      buffSize++;
    }
  }
  if(n < 0) {
    printf(1, "cmp: read error\n");
    exit();
  }
  
  if((fd2 = open(argv[2], O_RDWR)) < 0) {
    printf(1, "cmp: cannot open %s for reading\n", argv[2]);
    exit();
  }
  char *buffer2;
  buffer2 = (char*) malloc(1000000 * sizeof(char));
  int buffSize2 = 0;
  while((n = read(fd2, buf, sizeof(buf))) > 0)
  {
    for(i = 0; i < n; i++)
    {
      buffer2[buffSize2] = (char)buf[i];
      buffSize2++;
    }
  }
  if(n < 0) {
    printf(1, "cmp: read error\n");
    exit();
  }
  int lines = 1;
  if(buffSize > buffSize2) {
  	for(i = 0; i < buffSize; i++) {
  	  if(buffer[i] != buffer2[i]) {
  	    printf(1, "%s %s differ: byte %d, line %d\n", argv[1], argv[2], i + 1, lines);
  	    exit();
	  }
	  if(buffer[i] == '\n') lines++;
	}
  }
  printf(1, "_\n");
  exit();
}
```
Contoh saat command cmp dijalankan untuk membandingkan 2 file:

![cmp](https://user-images.githubusercontent.com/48936125/81304769-b511c400-90a7-11ea-9821-3914c157bf9a.jpg)
#

## tail
Command tail meminta kami untuk menampilkan bagian terakhir (10 baris secara default) dari satu atau lebih file atau data yang disalurkan.
```
Contoh: tail file1.c file2.c file3.txt
```
Maka akan ditampilkan 10 baris terakhir dari masing-masing ```file1.c```, ```file2.c```, dan ```file3.txt```.

Kami menggunakan buffer untuk membaca isi dari tiap file yang kemudian dicek jika menemukan karakter ```\n```, maka akan menambahkan nilai variabel ```lines``` yang menyimpan banyaknya baris yang ada pada file tersebut. Kemudian mencari startLine yang akan dicetak dengan menghitung ```lines - 10 + 1```, apabila ingin mengambil 10 baris terakhir, jika lines kurang dari 10, maka semua lines akan dicetak.

Semua yang dibaca dalam buffer akan di-*append* menjadi 1 string panjang, kemudian akan diloop ulang setelah mengetahui mulai baris ke berapa akan dicetak, di mana hanya menambahkan if index baris sekarang sudah lebih atau sama dengan startLine, maka baris tersebut akan dicetak.

Sama halnya seperti command lainnya, pada fungsi ```main``` juga dilakukan pengecekan argumen, harus minimal ada 1 argumen, kemudian argumennya tersebut harus merupakan file yang bisa dibaca (O_RDONLY).

Berikut implementasi tail:
```c
int main (int argc, char *argv[]) {
  int fd, i, j, n, lines = 0, toPrint, c = 0, startLine;
  if(argc <= 1) {
    printf(1, "tail: need argument at least 1 argument!");
    exit();
  }
  for(j = 1; j < argc; j++)
  {
    lines = 0, toPrint = 0, c = 0, startLine = 0;
    if((fd = open(argv[j], O_RDWR)) < 0) {
      printf(1, "tail: cannot open %s for reading\n", argv[j]);
      exit();
    }
    printf(1, "==> %s <==\n", argv[j]);
    char *buffer;
    buffer = (char*) malloc(1000000 * sizeof(char));
    int buffSize = 0;
    while((n = read(fd, buf, sizeof(buf))) > 0)
    {
      for(i = 0; i < n; i++)
      {
        buffer[buffSize] = (char)buf[i];
        buffSize++;
        if(buf[i] == '\n') lines++;
      }
    }
    if(n < 0) {
      printf(1, "tail: read error\n");
      exit();
    }
    if(lines < 10) toPrint = lines;
    else toPrint = 10;
    startLine = lines - toPrint + 1;
    for(i = 0; i < buffSize; i++)
    {
      if(c >= startLine) printf(1, "%c", buffer[i]);
      if(buffer[i] == '\n') c++;
    }
    printf(1, "\n");
    if(j != argc - 1) printf(1, "\n");
  }
  exit();
}
```
Contoh saat command tail dijalankan:

![tail](https://user-images.githubusercontent.com/48936125/81304618-80057180-90a7-11ea-91c2-597286a010a2.jpg)
#

## base64
Command base64 meminta kami untuk mengubah data menjadi bentuk base64 dan menampilkannya. Banyak argumen yang diterima hanya 1, yaitu file yang ingin diubah menjadi bentuk base64 untuk ditampilkan.
```
Contoh: base64 file.c
```
Maka akan ditampilkan hasil encode isi ```file.c``` dalam bentuk base64.

Base64 adalah teknik menerjemahkan data biner yang berbentuk ASCII, di mana untuk meng-encode base64, terdapat tabel yang dapat digunakan untuk menjadi panduan encoding, di mana mempunyai urutan angka sebagai berikut:
```
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
```

Salah satu faktor penting dalam encode base64, adalah mengambil setiap 6 bit biner untuk diencode, di mana agar tepat semua diencode, dapat **mengambil sebanyak tiga-tiga karakter ASCII (3 * 8 bit = 24 bit)** untuk mempermudah proses encoding. Hal ini dikarenakan FPB dari bit ASCII (8 bit) dan base64 (6 bit) adalah **24 bit**, di mana dibentuk dengan **3 karakter ASCII**.

Apabila ternyata jumlah karakter ASCII tidak habis dibagi 3 (banyak bit tidak pas diencode dalam bentuk base64), maka akan dilakukan yan disebut dengan *padding* berupa tanda ```=```.

Untuk membaca lebih lengkap mengenai teknik encoding base64, dapat dibaca [di sini](https://en.wikipedia.org/wiki/Base64)

Berikut implementasi encode base64:
```c
void
base64Encoder(char str[], int l)
{
  char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char *enc = (char *) malloc(1000000 * sizeof(char));
  int id, bits = 0, sisa = 0, val = 0, c = 0, temp;
  int i, j, k = 0;
  for(i = 0; i < l; i += 3)
  {
    val = 0, c = 0, bits = 0;
	for(j = i; j < l && j <= i + 2; j++)
	{
	  val <<= 8;
	  val |= str[j];
	  c++;
	}
	bits = c * 8;
	sisa = bits % 3;
	while(bits > 0)
	{
	  if(bits >= 6)
	  {
	    temp = bits - 6;
		id = (val >> temp) & 63;
		bits -= 6;
	  }
	  else
	  {
		temp = 6 - bits;
		id = (val << temp) & 63;
		bits = 0;
	  }
	  enc[k++] = key[id];
	}
  }
  for(i = 1; i <= sisa; i++) enc[k++] = '=';
  enc[k] = '\0';
  printf(1, "%s\n", enc);
}
```

Untuk mempermudah proses encoding, kami menggunakan bantuan operasi *bitwise* pada C (lebih hemat) untuk mengatasi konversi daari 8 bit (ASCII) menjadi 6 bit (base64). Untuk mendapatkan setiap karakter hasil encode, kami menggunakan perintah di-**AND**-kan dengan 63 (111111), agar terjiplak menjadi 6 bit secara keseluruhan tanpa mengubah nilainya, mengingat:
```
0 & 0 = 0
0 & 1 = 0
1 & 0 = 0
1 & 1 = 1
```

Kemudian di akhir, ada penambahan *padding* seperti yang sudah disebutkan, hanya perlu dilakukan dengan mengecek apakah banyak bit keseluruhan string yang akan diencode jika dimodulo 3 menyisakan berapa. Kemudian banyak sisa mod 3 itulah akan di-*append*-kan pada string hasil encode berupa ```=```.

Pada bagian ```main```, dilakukan pengecekan argumen apakah sudah benar yaitu memiliki 1 argumen yang dapat dibaca ```O_RDONLY```, kemudian untuk membaca isi file, digunakan loop while dan command ```open```, di mana berarti karakter dari file tersebut akan tersimpan pada **buffer**, di mana kami *append*-kan semua menjadi 1 string panjang pada variabel ```buffer``` agar menghasilkan encode yang benar dan sesuai.

Setelah pembacaan seluruh file, barulah memanggil fungsi ```base64Encoder```yang telah dibuat tadi dengan mem-*passing*-kan 2 value yaitu string ```buffer``` dan panjang string buffer yaitu ```buffSize```.

Berikut implementasi fungsi ```main```:
```c
int
main(int argc, char *argv[])
{
  int fd, i, n;
  if(argc <= 1) {
    printf(1, "base64: need 1 argument!\n");
    exit();
  }
  if((fd = open(argv[1], O_RDONLY)) < 0) {
    printf(1, "base64: cannot open %s for reading\n", argv[1]);
    exit();
  }
  char *buffer;
  buffer = (char*) malloc(1000000 * sizeof(char));
  int buffSize = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0)
  {
    for(i = 0; i < n; i++)
    {
      buffer[buffSize] = (char)buf[i];
      buffSize++;
    }
  }
  close(fd);
  if(n < 0) {
    printf(1, "base64: read error\n");
    exit();
  }
  base64Encoder(buffer, buffSize);
  exit();
}
```

Contoh saat command base64 dijalankan untuk *encoding* file README:

![base64](https://user-images.githubusercontent.com/48936125/81304519-5a786800-90a7-11ea-8c5b-61c5562c90ea.jpg)
#
