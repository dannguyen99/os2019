# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define buffer_size 10

typedef struct {
  char *name; // name of package
  char *version; // version of package
  char *author; // who created this package
} pkg;

pkg buffer[buffer_size]; // queue to transfer package from productor to user
int first = 0;
int last = 0;

// productor will produce package
void produce (pkg *i) {
  while ((first + 1) % buffer_size == last) {
    ; // wait for empty queue
  }
  memcpy(&buffer[first], i, sizeof(pkg));
  first = (first + 1) % buffer_size;
  printf("Log of send package: \nFirst: %d\nLast: %d\n",first,last);
}

// user will receive package
pkg *receive (void) {
  pkg *i = (pkg *) malloc(sizeof(pkg));
  while (first == last) {
    ; // wait for package put to queue
  }
  memcpy(i, &buffer[last], sizeof(pkg));
  last = (last + 1) % buffer_size;
  printf("Log of recieve package: \nFirst: %d\nLast: %d\n",first,last);
  return i;
}

// pack package to send to user
pkg pack (char *name, char *version, char *author) {
  pkg box;
  box.name = name;
  box.version = version;
  box.author = author;
  return box;
}

// garbage throw away box of package after unpack
void garbage (pkg *box) {
  free(box);
}

int main(){
  // factory side
  pkg sbox1 = pack("npm","1.0","Toval");
  produce(&sbox1);
  pkg sbox2 = pack("gcc","1.1","Toval");
  produce(&sbox2);
  pkg sbox3 = pack("bc","2.1","Toval");
  produce(&sbox3);

  // user side
  pkg *rrbox = receive();
  pkg rbox = *rrbox;
  printf("\nReceive package, show excited!\nPackage: %s\nVersion: %s\nAuthor: %s\n",rbox.name,rbox.version,rbox.author);
  garbage(rrbox);
  return 0;
}
