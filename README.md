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

## date

#

## tree

#

## cmp

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
