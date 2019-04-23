
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// atoi fonksiyonu ile ayni isi yapar char dizisi alip int e
// cevirir
int myAtoi(char *str, int j) 
{ 
    int res = 0; 
    for (int i = 0; i<j; ++i) 
        res = res*10 + str[i] - '0'; 
    return res; 
}




// tree icin struct yapisi
typedef struct tree{
    
    int usr_id;
    char *name_surname;
    int friend[20];
    struct tree *left;
    struct tree *right;
    
}Node;


//yeni node olusturma
Node * create_node(){
    Node * new_node;
    new_node = (Node *)malloc(sizeof(Node));
    new_node->left=NULL;
    new_node->right=NULL;
    return new_node;
}
             
 // agaca node eklemesi yapar root nodu ve eklenecek nodu parametre
// olarak alir, recursive
Node * insert_node(Node *root, Node * tmp){
    if(tmp->usr_id < 0){     
    }
    else if(root == NULL){
        root = create_node();
        root->usr_id = tmp->usr_id;
        root->name_surname = tmp->name_surname;
        int i=0;
        while(tmp->friend[i] != 0){
            root->friend[i] = tmp->friend[i];
            i++;
        }
        return root;
    }
    else{
        if(root->usr_id > tmp->usr_id){
            root->left = insert_node(root->left , tmp);
        }
        else{
            root->right = insert_node(root->right, tmp);
        }
    }
    
    return root;
}


//friends fonksiyonu icin yazdigim arama fonksiyonıu
//root ve id alir donus olarak node adresi dondurur

Node * contains_friends(Node * root, int id) 
{ 
    if(root == NULL){
        printf("id bulunamadı\n");
        return 0;
    }
    if (root->usr_id == id){ 
        printf("%s\n",root->name_surname);
       return root; 
    } 
    if (root->usr_id < id){
        
       return contains_friends(root->right, id); 
    }
  
    return contains_friends(root->left, id); 
}



//node uzerinde verilen id yi arar
// buldugu id'li node'u dondurur
Node * contains(Node * root, int id) 
{ 
    if(root == NULL){
        printf("id bulunamadı\n");
        return 0;
    }
    if (root->usr_id == id){ 
        printf("bulundu :  ");
        printf("%s\n",root->name_surname);
       return root; 
    }
    
    if (root->usr_id < id){
        printf("karsilastiriliyor : %d\n",root->usr_id);
        
       return contains(root->right, id); 
    }
  
    printf("karsilastiriliyor : %d\n",root->usr_id);
    return contains(root->left, id); 
}



// verilen id deki node'u bulup sonra bu node'un arkadaslarini 
// tree de arayip bulur ekrana yazar
//donus tipi node adresi

Node * friends(Node *root, int id,Node ** rootptr ){
    if(root == NULL){
        printf("id bulunamadı \n");
        return 0;
    }
    if (root->usr_id == id){ 
        int i=0;
        while(root->friend[i] != 0){
            contains_friends((*rootptr), root->friend[i]);
            i++;
        }
        return root;
    }
    
    if (root->usr_id < id){
        
       return friends(root->right, id, rootptr); 
    }
  
    return friends(root->left, id, rootptr); 
}



//tree yi inorder dolasip id leri ekrana yazdirir
void printInorder (Node * root) 
{ 
    if (root == NULL) 
        return; 
    printInorder(root->left); 
    printf("%d ", root->usr_id); 
    printInorder(root->right); 

}

// tree yi inorder dolasip isimleri yazar 
void printInorderName (Node * root) 
{ 
    if (root == NULL) 
        return; 
    printInorderName(root->left); 
    printf("%s\n", root->name_surname); 
    printInorderName(root->right); 

}

// tree deki node larin toplam sayisini verir
//node sayisini dondurur

int size(Node * root){
    if(root == NULL){
        return 0;
    }
    return size(root->left) + size(root->right) + 1;
    
}

// verilen id deki node bulur 
// bulunan node u printInorderName fomksiyonuna gonderir
// o fonksiyonda kendisine gelen node un altindaki 
// butun node lari inorder ekrana yazae

void printNext(Node * root,int id){
    Node *curr = NULL;
    if(root == NULL){
        printf("bulunamadi");
        return -1;
    }
    if(root->usr_id == id){
        
        curr = root;
        printInorderName(curr);
        
    }
    else if(id < root->usr_id){
        printNext(root->left, id);
        
    }
    else{
        printNext(root->right,id);
    }
    
}



 // kendisine verilen id deki node dan buyuk idli node lari
// yazdirir
 void printGreater(Node * root,int id){
	if(root == NULL) return;
	printGreater(root->left,id);
		if(id < root->usr_id){
		printf("id: %d\n",root->usr_id);
		printf("Isim Soyisim : %s \n",root->name_surname);
		}
	printGreater(root->right,id);
}
 
 
 // tree deki minimum degeri tasiyan node u dondurur
 Node * min(Node * root) { 
     
     Node *curr;
     curr=root;
    while (curr->left != NULL) 
        curr = curr->left; 
  
    return curr; 
}
 
 // new user insert eden fonksiyon
 Node * insertNewUser(Node *root, int id){
    if(root == NULL){
        root = create_node();
        root->usr_id = id;
        //root->friend[] = NULL;
        root->name_surname = NULL;
        return root;
    }
    else{
        if(id < root->usr_id){
            root->left = insertNewUser(root->left,id);
        }
        else{
            root->right = insertNewUser(root->right, id);
        }
    }
    return root;
}

 
 
 
 // treeden node siler
 Node *deleteUser(Node *root,int kimlikNo){
	if(root == NULL) return root;
	if(kimlikNo < root->usr_id){
		root->left = deleteUser(root->left,kimlikNo);
	}
	else if(kimlikNo > root->usr_id){
		root->right = deleteUser(root->right,kimlikNo);
	}
	else{
		if(root->left == NULL){
			struct node *temp = root->right;
			free(root);
			return temp;
		}
		else if(root->right == NULL){
			Node *temp = root->left;
			free(root);
			return temp;
		}
		Node *temp = min(root->right);
		root->usr_id = temp->usr_id;
    strcpy(root->name_surname,temp->name_surname);
    strcpy(root->friend,temp->friend);

		root->right = deleteUser(root->right,temp->usr_id);
	}
	return root;
}
 
 


int main(){
    
    Node *root = NULL; // root
    Node ** rootptr;   // root un adresini tutan pointer
    rootptr=&root;
    
    FILE *f; // dosya 
    char str[100];// fgets ile okuma yapilirken okunanlarin tutuldugu dizi
    char id_d[3] = {0,0};//ilk virgule kadar olanlari tutan dizi
    int id;//ilk virgule kadar olanlarin int tipine donustuıkten sonra
    // tutulacaklari yer
    char name[30];//ilk virgul ile 2. virgul arasinda tutlanlarin oldugu dizi
    char arr[3];//ikinci virgulden sonra kisa cizgi arasinda olarlarin tutuldugu dizi
    int sayi;// arr dizisinde olan elemanlar her seferinde myAtoi fonksiyonu ile buna atilir
    int i;
    int j;
    int p;
    int count_insert = 0; // tree size in bulmak icin kullanilan degisken
    int id_al;// kullanicidan alinan deger
    
    printf("agac olusturuluyor\n");
    f=fopen("Input.txt","r");
                if(f==NULL){
                printf("acilamadi");
                
                 }
                
/*burdan sonra dosya acilacak ve ilk virgul gorulene kadar olan dagerler
 id_d adli diziye atilacaklar , bundan sonra myAtoi fonksiyonu ile
 bu dizideki degerler id adli kullanicilarin idsini tutan degiskene
 atilacak,ve bu deger de tmp adli node un usr_id adli degiskenine atilacak*/
    
/*daha sonra 1. virgul ve 2. virgul arasindakiler(eger yoksa new line)
 arasindakiler name adli diziye atilir daha sonra bu name dizisinin
 sonuna null eklenir ve bu dizi tmp node un name_surname degiskenine atanir*/
    
 /*bundan sonra 2. virgulden sonrakiler aralarindaki kisa cizgiden 
  kurtarilarak sirayla arr adli diziye atanir sonra bu dizideki deger
  myAtoi fonksiyonu ile int cevrilip sayi adli degiskene atanir daha sonra bu sayi
  tmp node nun friend dizisine atanir*/
                while(fgets (str, 100, f) != NULL){
                    Node * tmp;
                    tmp = create_node();
                    i=0;
                    j=0;
                    memset(id_d,0,3);
                    while(str[i] != ',' && str[i] != '\n' ){
                        id_d[j] = str[i];
                        j++;
                        i++;
                    }
                    i++;
    
                    
                    id=myAtoi(id_d,j);
                    tmp->usr_id=id;
    
   
                    j=0;
                    while(str[i] != ',' && str[i] != '\n'){
                        name[j] = str[i];
                        j++;
                        i++;
                    }
                    j++;
                    name[j] = '\0';
                    i++;
                    tmp->name_surname = (char *)malloc(sizeof(char)*30);
                    strcpy(tmp->name_surname,name);
    
 
                    int k;
                     k=0;
                    while(str[i] != '\0'){
                        p=0;
                        j=i;
                        memset(arr,0,3);
                        while(str[j] != '-' && str[j] != '\n'){
                            arr[p] =str[j];
            
                            p++;
                            j++;
                            i++;
                        }
                        sayi = atoi(arr);
                        tmp->friend[k] = sayi;
                        i++;
                        k++;
                        }
                    k++;
                    tmp->friend[k] = 0;
    
    //tmp nodu burda tam olarak olusmus olur
    
                    memset(name,'\0',sizeof(name));
                    root = insert_node(root,tmp);
                    count_insert++;
                    if(count_insert % 10 == 0){
                        printInorder(root);
                        printf("\n");
                    }
  
                }
    printf("\n\n");
    
    printf("olusturulan tree \n\n");
    printInorder(root);
    printf("\n\n\n");
    
    while(1){
        
        printf("yapacaginiz islem icin islem karsisinda bulunan numarayi seciniz \n\n\n");
        printf("yeni eleman eklemek icin (insertNewUser) :                  1\n");
        printf("agactan eleman silmek icin (deleteUser) :                   2 \n");
        printf("agacta arama yapmak icin (contains) :                       3\n");
        printf("arkadaslarini bulmak icin (friends) :                       4\n");
        printf("agactaki eleman sayisi icin (size) :                        5\n");
        printf("printNext fonksiyonu icin  :                                6\n");
        printf("printGreater fonksiyonu icin :                              7\n");
        printf("agaci yazdirmak icin :                                      8\n");
        printf("exit icin :                                                 9\n\n\n");
        
        int secim;
        printf("seciminizi giriniz : ");
        scanf("%d",&secim);
        
        
        
        switch (secim){
            case 1:
                printf("eklemek istedigniz id yi giriniz ");
                scanf("%d",&id_al);
                printf("ekleme yapmadan once tree\n\n");
                printInorder(root);
                printf("\n");
                insertNewUser(root,id_al);
                printf("\n");
                printf("ekleme yapildiktan sonra tree\n\n");
                printInorder(root);
                printf("\n");
                break;
                
                
            case 2:
                printf("silmek istedigniz id ' yi giriniz :  ");
                scanf("%d",&id_al);
                
                printf("silmeden once tree \n");
                printInorder(root);
                deleteUser(root, id_al);
                printf("\n");
                printf("sildikten sonra\n");
                printInorder(root);
                printf("\n");
                break;
                
            case 3:
                printf("aramak istediginiz id yi giriniz :  ");
                scanf("%d",&id_al);
                printf("\n");
                contains(root,id_al);
                printf("\n");
                break;
                
                
            case 4:
                printf("istediginiz id nin arkadaslarini bulmak icin id giriniz:  ");
                scanf("%d",&id_al);
                friends(root,id_al,rootptr);
                printf("\n");
                break;
                
            case 5:
                printf("agactaki eleman sayisi = %d ",size(root));
                printf("\n");
                break;
                
            case 6:
                printf("printNext fonksiyonu icin id giriniz:  ");
                scanf("%d",&id_al);
                printNext(root,id_al);
                printf("\n");
                break;
                
            case 7:
                printf("printGreater fonksiyonu icin id giriniz: ");
                scanf("%d",&id_al);
                printGreater(root,id_al);
                printf("\n");
                break;
                
            case 8:
                printf("agac yazdiriliyor ...\n\n");
                printInorder(root);
                printf("\n");
                break;
            case 9:
                return 0;
                
            default:
                printf("yanlis secim yaptiniz \n\n\n");
                break;
                
        }
        
  
        
        
    }
    
    
    return 0;
}
