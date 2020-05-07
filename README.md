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
1. UPROGS, ditambahkan nama command yang dibuat dalam format ```_/command```.
2. EXTRA, ditambahkan file .c yang memuat implementasi command tersebut dalam format ```command.c```.
#

## date

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

Untuk menampilkan agar struktur tree lebih mirip command tree sebenarnya pada linux, maka kami menggunakan ```|-- ``` untuk menandakan setiap ada file/direktori yang ingin ditampilkan, dikarenakan indentasi itu mengambil 4 spasi, maka untuk tiap level kedalaman, nanti akan diberikan spasi sebanyak kedalaman * 4, agar indentasi sesuai dan terlihat rapi:

gambar

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
#
