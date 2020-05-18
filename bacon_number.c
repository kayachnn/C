#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Film_sayisi 23719
#define Oyuncu_sayisi 214177
#define R 37


typedef struct Film
{
	char *isim;
	int oyuncu_sayisi;
	struct Oyuncu **oyuncular;
        int size; // realloc icin kullanilacak
}Film;

typedef struct Oyuncu
{
	char *name;
	int film_sayisi;
	struct Film **filmler;
        int size;  // realloc icin kullanilacak
        int visited;
}Oyuncu;

Oyuncu* oyuncu_ara(Oyuncu** oyuncular, char* oyuncu_name);
void bfs(Oyuncu ** oyuncular, char* oyuncu1_name, char* oyuncu2_name);
Film * add_film(char *film_name,Film ** filmler);
long long int calculate_key(char*);
void add_oyuncu(Oyuncu** oyuncular, char *oyuncu_name, Film* film);
int power(int x, int y);


unsigned  int h1_oyuncu(long long int);  // ilk hash fonksiyonu
unsigned  int h2_oyuncu(long long int);  // ikinci hash fonksiyonu
unsigned  int h_oyuncu(long long int, int);  // main hash fonksiyonu

unsigned  int h1_movie(long long int );
unsigned  int h2_movie(long long int );
unsigned  int h_movie(long long int, int);


int main(void)
{   
    int i;
    long int j;
    unsigned int filmIndex = 0;   // film dizisinde kullanilacak film indexi
    Film** filmler;      //film adreslerinin tutuldugu dizi
    Oyuncu** oyuncular;  // oyuncu adreslerinin tutuldugu dizi
    filmler = (Film**)malloc(sizeof(Film*) * Film_sayisi);
    oyuncular = (Oyuncu**)malloc(sizeof(Oyuncu*)* Oyuncu_sayisi);
    unsigned char movie_name[256];  // filmin tutuldugu degisken
    unsigned char oyuncu_name[256];  // oyucunun tutuldugu degisken
    long long int oyuncu_sayisi = 0;
    //unsigned int film_sayisi;
    unsigned char file_buffer[4096]; // her filmle ilgili butun bilgileri tutacak buffer
    
    
    for(i=0;i<Film_sayisi;i++)
    {
        filmler[i] = NULL;
    }
    
    for(i=0;i<Oyuncu_sayisi;i++)
    {
    	oyuncular[i] = NULL;
	}
    char *input_filename=malloc(sizeof(char)*20);
    memset(input_filename,'\0',20);
    input_filename = "input-mpaa.txt";
    
    
    // file okuma islemi basliyor

    FILE *input_file;
    input_file = fopen(input_filename,"r");

    if(input_file == NULL)
    {
        printf("dosya acilamadi");
        exit(-1);
    }
    
    memset(file_buffer,'\0',4096);
    while(fgets(file_buffer,4096,input_file) != NULL)
    {
	    memset(movie_name,'\0',256);
	    i=0;
            // paranteze kadar okuyup film ismini buluyor
	    while( file_buffer[i] != ')')
	    {
	    	movie_name[i] = file_buffer[i];
	        i++;
	    }
            movie_name[i] = ')';
            //film isminin sondaki bosluk kaldiriliyor
	    //printf("film:%s\n",movie_name);
	
        // bu noktada film ismi okunmus oluyor
        // okunan film icin struct olusturulup filmler adli diziye atanacak
            
             Film * movie = add_film(movie_name, filmler);
            
    
	
	
        //oyuncu_sayisi = 0;
        while(file_buffer[i] != '\n')		
        {
					
            // oyuncu isimlerini oku	
            i++;	
            j=0;
            memset(oyuncu_name,'\0',256);
            while( file_buffer[i] != '/' && file_buffer[i] != '\n')
            {
                    oyuncu_name[j] = file_buffer[i];
                    j++,i++;
            }

            //printf("ad: %s\n",oyuncu_name);

            // oyuncu olusturulup oyuncular dizisine ekleniyor
            add_oyuncu(oyuncular, oyuncu_name,movie);
            oyuncu_sayisi++;
		
        }
        memset(file_buffer,'\0',4096);
    }
         
    // oyunculari ve filmlerini yazdir
   /* int k;
    for(j=0;j<=Oyuncu_sayisi;j++)
    {
        if(oyuncular[j] != NULL)
        {
            printf("%ld oyuncu: %s\n",j,oyuncular[j]->name);
            k=0;
            while(k<oyuncular[j]->film_sayisi)
            {
                printf("-----film:%s\n",oyuncular[j]->filmler[k]->isim);
                k++;
            }
        }
        
    }*/
    
    //bfs(oyuncular,"Bacon, Kevin","Schwarzenegger, Arnold");
    Oyuncu* bacon = oyuncu_ara(oyuncular,"Bacon, Kevin");
    printf("%s,%d\n\n",bacon->name,bacon->film_sayisi);
    
    for(i=0;i<bacon->film_sayisi;i++)
    {
        printf("%s\n",bacon->filmler[i]->isim);
    }
    printf("\n\n\n");
     
    bfs(oyuncular,"Bacon, Kevin","Schwarzenegger, Arnold");
    bfs(oyuncular,"Nicholson, Jack","Streep, Meryl");
     bfs(oyuncular,"Connelly, Christopher","Streep, Meryl");
     bfs(oyuncular,"Connelly, Christopher","Catenacci, Luciano");
     
    
   
    
    return 0;
}


Oyuncu* oyuncu_ara(Oyuncu** oyuncular, char* oyuncu_name)
{
    Oyuncu* oyuncu = malloc(sizeof(Oyuncu));
    int i;
    long long int key = calculate_key(oyuncu_name);
    long int value;
    i=0;
    do
    {
        value = h_oyuncu(key, i);
        i++;
    }
    while(strcmp(oyuncu_name, oyuncular[value]->name) != 0 && i<Oyuncu_sayisi);
    if(i<Oyuncu_sayisi)
    {
        oyuncu = oyuncular[value];
        return oyuncu;
    }
    else
        return NULL;
    
}
void bfs(Oyuncu ** oyuncular, char* oyuncu1_name, char* oyuncu2_name)
{
    Oyuncu * oyuncu1 = malloc(sizeof(Oyuncu));
    Oyuncu * oyuncu2 = malloc(sizeof(Oyuncu));
    long long int q_size = 300000;
    long long index;
    int top=0;
    int i,j,k;
    int isfound;
    int Bacon_sayisi=0;
    int derinlik=0;
    
    
    // oyuncular olusturuluyor
    oyuncu1 = oyuncu_ara(oyuncular,oyuncu1_name);
    oyuncu2 = oyuncu_ara(oyuncular,oyuncu2_name);
    
    // queue olusturuluyor
    Oyuncu** queue = malloc(q_size*sizeof(Oyuncu*)); // gueue olusturuluyor
    for (index = 0; index < q_size; index++){
        queue[index]=NULL;
    }
    if(oyuncu1==NULL||oyuncu2==NULL){
        printf("oyuncu bulunamadi\n");
    }
    else
    {
        queue[0]=oyuncu1; // gueue ye ilk oyuncu ataniyor
        index=0;
        top=1;
        isfound=0;
        while(queue[index]!=oyuncu2&&queue[index]!=NULL){ // aranan oyuncuyu queue de gorene kadar queue bosalana kadar devam et
            for(i=0;i<queue[index]->film_sayisi;i++){
                for(j=0;j<queue[index]->filmler[i]->oyuncu_sayisi;j++){
                    if(queue[index]->filmler[i]->oyuncular[j]->visited==1){    
                        continue;
                    }
                    if(top>q_size){  // queuee nin boyutu yetersiz kalirsa arttir
                        q_size *=2;
                        Oyuncu** temp;
                        temp = (Oyuncu**)realloc(queue,(q_size*sizeof(Oyuncu*)));
                        if(temp==NULL){
                            printf("\nqueue nin boyutu arrttirilamiyor:%lld\n",q_size);
                            exit(-1);
                        }
                        queue=temp;
                        for(k=q_size/2;k<q_size;k++)
                            queue[k]=NULL;
                    }
                    queue[top++]=queue[index]->filmler[i]->oyuncular[j];
                    queue[index]->filmler[i]->oyuncular[j]->visited=1;
                }
            }
            if(index==derinlik){
                derinlik=top-1;
                Bacon_sayisi++;
            }
            index++;
        }
        
        for(i=0;i<Oyuncu_sayisi;i++){
            if(oyuncular[i] != NULL)
            {
                oyuncular[i]->visited = 0;
            }
        }
        
        if(queue[index]==NULL){
            printf("%s - %s: iliski bulunamadi\n",oyuncu1->name,oyuncu2->name);
        }else
            printf("%s - %s : %d\n",oyuncu1->name,oyuncu2->name,Bacon_sayisi);
        }
        free(queue);
    
}

Film * add_film(char *film_name,Film ** filmler)
{   
    //filmin hash degeri bulunup filmler tablosuna yerlestirilecek
    int i=0;
    long long int key = calculate_key(film_name);
    long int value  = h_movie(key,i);

    Film* yeni_film = NULL;
    yeni_film = (Film *)malloc(sizeof(Film));
    yeni_film->isim = malloc(sizeof(char)*256);
    memset(yeni_film->isim, '\0',256);
    strcpy(yeni_film->isim, film_name);
    yeni_film->oyuncu_sayisi = 0;
    yeni_film->size = 10;
    yeni_film->oyuncular =NULL;
    yeni_film->oyuncular = (Oyuncu **)malloc(sizeof(struct Oyuncu*) * yeni_film->size);
    filmler[value] = yeni_film;
    //printf("hash degeri: %ld\n",value);
    
    if( filmler[value] == NULL)
    {   
        //printf("value: %ld\n",value);
        filmler[value] = yeni_film;
    }
    else
    {
        i=1;
        do
        {      
            value = h_movie(key, i);
            i++;
        }while(filmler[value] != NULL  &&  i<Film_sayisi);
     //   printf("value degeri:%ld\n",value);
    }
    
    
    if(i<Film_sayisi)
    {
        filmler[value] = yeni_film;
    }
    else
    {
        printf("film table dolu\n");
        exit(-1);
    }
return yeni_film;     
}

void add_oyuncu(Oyuncu** oyuncular, char *oyuncu_name, Film* film)
{
    // oyuncunun hash degeri hesaplaniyor	 
    int i =0;
    long long int key = calculate_key(oyuncu_name);
    long int value  = h_oyuncu(key,i);
    
    Oyuncu* oyuncu = NULL;
    oyuncu = (Oyuncu *)malloc(sizeof(Oyuncu));
    oyuncu = oyuncular[value];
    
    // hash null ise oyuncu unique olur
    // yeni oyuncu olusturulur 
    if( oyuncu == NULL )
    {   
        Oyuncu * temp_oyuncu = malloc(sizeof(Oyuncu));
        temp_oyuncu->name = (char *)malloc(sizeof(char)*256);
        memset(temp_oyuncu->name,'\0',256);
        strcpy(temp_oyuncu->name, oyuncu_name);
        temp_oyuncu->film_sayisi = 1;
        temp_oyuncu->size = 10;
        temp_oyuncu->filmler = (Film **)malloc(sizeof(Film*) * temp_oyuncu->size);
        temp_oyuncu->filmler[0] = film;
        temp_oyuncu->visited = 0;
        oyuncular[value] = temp_oyuncu;
        oyuncu = temp_oyuncu;
    }
    else if(strcmp(oyuncu_name, oyuncular[value]->name) == 0)
    {  
        oyuncu->film_sayisi += 1;
        if(oyuncu->film_sayisi > oyuncu->size)
        {
            oyuncu->size *=2;
            Film ** temp_film = realloc(oyuncu->filmler,(oyuncu->size* sizeof(Film*)));
            if(temp_film != NULL)
            {
                oyuncu->filmler = temp_film;
            }
            else
            {
                printf("oyuncu dizisi genisletilemedi\n");
                exit(-1);
            }
        }
        oyuncu->filmler[oyuncu->film_sayisi-1]=film;
    }
    else
    {                    
        i=1;
        do
        {
            value = h_oyuncu(key,i);
            i++;
        }while(oyuncular[value] != NULL && strcmp(oyuncular[value]->name ,oyuncu_name) != 0 && i<Oyuncu_sayisi);

        //printf("%s\n",oyuncular[value]->name);
        // ayni oyuncu varsa bulana kadar devam etcez yoksa null bulana kadar devam etcez i = oyuncu sayisi olursa tablo doludur
        if(i == Oyuncu_sayisi)
        {
            printf("hash table dolu");
        }
        else if(oyuncular[value] == NULL)  // ayni oyuncuyu bulduk
        {
           // printf("bos goz bulundu");
            Oyuncu * temp_oyuncu = malloc(sizeof(Oyuncu));
            temp_oyuncu->name = (char *)malloc(sizeof(char)*256);
            memset(temp_oyuncu->name,'\0',256);
            strcpy(temp_oyuncu->name, oyuncu_name);
            temp_oyuncu->film_sayisi = 1;
            temp_oyuncu->size = 10;
            temp_oyuncu->filmler = (Film **)malloc(sizeof(Film*) * temp_oyuncu->size);
            temp_oyuncu->filmler[0] = film;
            temp_oyuncu->visited =0;
            oyuncular[value] = temp_oyuncu;
            oyuncu = temp_oyuncu;
        }

        else if(strcmp(oyuncu_name, oyuncular[value]->name) == 0) // tabloda bos yer bulundu oraya yeni oyuncu yaratilip eklenecek
        {
            oyuncu->film_sayisi += 1;
            if(oyuncu->film_sayisi > oyuncu->size)
            {
                oyuncu->size *=2;
                Film ** temp_film = realloc(oyuncu->filmler,(oyuncu->size* sizeof(Film*)));
                if(temp_film != NULL)
                {
                    oyuncu->filmler = temp_film;
                }
                else
                {
                    printf("oyuncu dizisi genisletilemedi\n");
                    exit(-1);
                }
            }
            oyuncu->filmler[oyuncu->film_sayisi-1]=film;
        }
    }
    
    film->oyuncu_sayisi +=1;
    if(film->oyuncu_sayisi > film->size) // fiilmin oyuncu sayisi size dan buyuk mu
    {
        film->size +=10;
        Oyuncu ** temp_film = realloc(film->oyuncular,(film->size * sizeof(Oyuncu*)));
        if(temp_film != NULL)
        {
            film->oyuncular = temp_film;
        }
        else
        {
            exit(-1);
        }
    } 
    film->oyuncular[film->oyuncu_sayisi-1] = oyuncu;  
}


long long int  calculate_key(char* oyuncu_film_adi)
{
    // oyuncu isminin key degerini bulur

    long long int key;
    int i;
    int count; // kelimelerin uzunlugunu tutuyor
    count = strlen(oyuncu_film_adi);
    key=0;
    
            
    // key hesaplama kısmı
    for(i=0;i<count;i++)
    {
        key += oyuncu_film_adi[i] * power(R,count-i-1);
        key %= 1000007;
    }
    key %= 1000007;
    return abs(key);
}





/* Iterative Function to calculate (x^y)%p in O(log y) */
int power(int x, int y) 
{   //(x^y)%p O(logn) de hesaplamak icin fonksşyon
    int p = 1000007;
    int res = 1;      // Initialize result 
  
    x = x % p;  // Update x if it is more than or  
                // equal to p 
  
    while (y > 0) 
    { 
        // If y is odd, multiply x with result 
        if (y & 1) 
            res = (res*x) % p; 
  
        // y must be even now 
        y = y>>1; // y = y/2 
        x = (x*x) % p;   
    } 
    return res; 
}




unsigned int h1_oyuncu(long long int key){
    return abs(key % Oyuncu_sayisi);
}

unsigned int h2_oyuncu(long long int key){
    return 1 + abs(key % (Oyuncu_sayisi -1));
}

unsigned int h_oyuncu(long long int key, int i){
    return (h1_oyuncu(key) + i*h2_oyuncu(key)) % Oyuncu_sayisi;
}


unsigned int h1_movie(long long int key){
    return abs(key % Film_sayisi);
}

unsigned int h2_movie(long long int key){
    return 1 + abs(key % (Film_sayisi -1));
}

unsigned int h_movie(long long int key, int i){
    return (h1_movie(key) + i*h2_movie(key)) % Film_sayisi;
}














