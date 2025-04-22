#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 1000
#define MAX_LINE_LENGTH 100

// Renk tanimlamalari (Kirmizi-Siyah Agac icin)
typedef enum Color {
    RED,
    BLACK
} Color;

// Kullanici yapisi
typedef struct User {
    int id;
    struct Relationship* relationships;
} User;

// Iliski yapisi (Komsuluk listesi elemani)
typedef struct Relationship {
    int friendId;
    struct Relationship* next;
} Relationship;

// Kirmizi-Siyah Agac dugumu
typedef struct RBNode {
    int userId;
    User* user;
    Color color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
} RBNode;

// Global degiskenler
User* users[MAX_USERS];
int userCount = 0;
RBNode* rbRoot = NULL;
RBNode* NIL;

// Fonksiyon prototipleri
User* createUser(int id);
void addRelationship(User* user, int friendId);
void loadDataFromFile(const char* filename);

// Kirmizi-Siyah Agac fonksiyonlari
RBNode* createRBNode(int userId, User* user);
void leftRotate(RBNode* x);
void rightRotate(RBNode* y);
void rbInsertFixup(RBNode* z);
void rbInsert(int userId, User* user);
RBNode* rbSearch(RBNode* root, int userId);
void freeRBTree(RBNode* node);

// Analiz fonksiyonlari
void dfsSearch(int userId, int distance);
void findCommonFriends(int userId1, int userId2);
void detectCommunities();
void calculateInfluence(int userId);
void printRelationshipTree();

// Yardimci fonksiyonlar
User* getUserById(int id);
void dfsHelper(int userId, int targetId, int currentDistance, int maxDistance, int* visited);
int* getCommonFriends(int userId1, int userId2, int* count);
void dfsCommunity(int userId, int communityId, int* visited, int* community);
int getInfluenceScore(int userId, int depth, int* visited);
void printUserRelationships(User* user);

// Yeni bir kullanici olustur
User* createUser(int id) {
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Bellek hatasi: Kullanici olusturulamadi\n");
        exit(1);
    }
    newUser->id = id;
    newUser->relationships = NULL;
    return newUser;
}

// Kullaniciya arkadaslik iliskisi ekle
void addRelationship(User* user, int friendId) {
    if (user == NULL) return;
    
    // Zaten var mi kontrol et
    Relationship* current = user->relationships;
    while (current != NULL) {
        if (current->friendId == friendId) return;
        current = current->next;
    }
    
    // Yeni iliski olustur
    Relationship* newRelationship = (Relationship*)malloc(sizeof(Relationship));
    if (newRelationship == NULL) {
        printf("Bellek hatasi: Iliski olusturulamadi\n");
        return;
    }
    newRelationship->friendId = friendId;
    newRelationship->next = user->relationships;
    user->relationships = newRelationship;
}

// Kullaniciyi ID'ye gore bul
User* getUserById(int id) {
    int i;
    for (i = 0; i < userCount; i++) {
        if (users[i]->id == id) return users[i];
    }
    return NULL;
}

// Veri dosyasini yukle
void loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi: %s\n", filename);
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[strcspn(line, "\n")] = 0; // Satir sonu karakterini kaldir
        
        char command[10];
        int id1, id2;
        
        if (sscanf(line, "%s %d", command, &id1) == 2) {
            if (strcmp(command, "USER") == 0) {
                // Kullanici zaten var mi kontrol et
                User* existingUser = getUserById(id1);
                if (existingUser == NULL) {
                    users[userCount] = createUser(id1);
                    // Kirmizi-Siyah agaca ekle
                    rbInsert(id1, users[userCount]);
                    userCount++;
                }
            } else if (strcmp(command, "FRIEND") == 0 && sscanf(line, "%s %d %d", command, &id1, &id2) == 3) {
                User* user1 = getUserById(id1);
                User* user2 = getUserById(id2);
                
                if (user1 == NULL || user2 == NULL) {
                    printf("Hata: Kullanici bulunamadi (%d veya %d)\n", id1, id2);
                    continue;
                }
                
                // Cift yonlu iliski ekle
                addRelationship(user1, id2);
                addRelationship(user2, id1);
            }
        }
    }
    
    fclose(file);
}

// Kirmizi-Siyah Agac dugumu olustur
RBNode* createRBNode(int userId, User* user) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    if (node == NULL) {
        printf("Bellek hatasi: RB dugumu olusturulamadi\n");
        exit(1);
    }
    node->userId = userId;
    node->user = user;
    node->color = RED;
    node->left = NIL;
    node->right = NIL;
    node->parent = NIL;
    return node;
}

// Sol rotasyon
void leftRotate(RBNode* x) {
    if (x == NIL || x->right == NIL) return;
    
    RBNode* y = x->right;
    x->right = y->left;
    
    if (y->left != NIL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NIL) {
        rbRoot = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

// Sag rotasyon
void rightRotate(RBNode* y) {
    if (y == NIL || y->left == NIL) return;
    
    RBNode* x = y->left;
    y->left = x->right;
    
    if (x->right != NIL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == NIL) {
        rbRoot = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Kirmizi-Siyah Agac duzeltme
void rbInsertFixup(RBNode* z) {
    while (z->parent != NIL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            
            if (y != NIL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            
            if (y != NIL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
        
        if (z == rbRoot) break;
    }
    
    rbRoot->color = BLACK;
}

// Kirmizi-Siyah Agac ekleme
void rbInsert(int userId, User* user) {
    RBNode* z = createRBNode(userId, user);
    RBNode* y = NIL;
    RBNode* x = rbRoot;
    
    while (x != NIL) {
        y = x;
        if (z->userId < x->userId) {
            x = x->left;
        } else if (z->userId > x->userId) {
            x = x->right;
        } else {
            free(z);
            return;
        }
    }
    
    z->parent = y;
    
    if (y == NIL) {
        rbRoot = z;
    } else if (z->userId < y->userId) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    z->left = NIL;
    z->right = NIL;
    z->color = RED;
    
    rbInsertFixup(z);
}

// Kirmizi-Siyah Agac arama
RBNode* rbSearch(RBNode* root, int userId) {
    if (root == NIL || root->userId == userId) {
        return root;
    }
    
    if (userId < root->userId) {
        return rbSearch(root->left, userId);
    } else {
        return rbSearch(root->right, userId);
    }
}

// Kirmizi-Siyah Agac bellegini temizleme
void freeRBTree(RBNode* node) {
    if (node == NIL) return;
    
    freeRBTree(node->left);
    freeRBTree(node->right);
    
    free(node);
}

// DFS yardimci fonksiyonu
void dfsHelper(int userId, int targetId, int currentDistance, int maxDistance, int* visited) {
    if (currentDistance > maxDistance || visited[userId]) return;
    
    visited[userId] = 1;
    
    if (userId != targetId && currentDistance > 0) {
        printf("Kullanici %d (Mesafe: %d)\n", userId, currentDistance);
    }
    
    User* user = getUserById(userId);
    if (user == NULL) return;
    
    Relationship* current = user->relationships;
    while (current != NULL) {
        if (!visited[current->friendId]) {
            dfsHelper(current->friendId, targetId, currentDistance + 1, maxDistance, visited);
        }
        current = current->next;
    }
}

// DFS ile belirli mesafedeki arkadaslari bulma
void dfsSearch(int userId, int distance) {
    int visited[MAX_USERS] = {0};
    printf("Kullanici %d'den en fazla %d mesafedeki arkadaslar:\n", userId, distance);
    dfsHelper(userId, userId, 0, distance, visited);
}

// Ortak arkadaslari bulma
int* getCommonFriends(int userId1, int userId2, int* count) {
    static int commonFriends[MAX_USERS];
    *count = 0;
    
    User* user1 = getUserById(userId1);
    User* user2 = getUserById(userId2);
    
    if (user1 == NULL || user2 == NULL) {
        return commonFriends;
    }
    
    // Kullanici 1'in arkadaslarini tara
    Relationship* rel1 = user1->relationships;
    while (rel1 != NULL) {
        int friendId = rel1->friendId;
        
        // Kullanici 2'nin de arkadasi mi kontrol et
        Relationship* rel2 = user2->relationships;
        while (rel2 != NULL) {
            if (rel2->friendId == friendId) {
                commonFriends[(*count)++] = friendId;
                break;
            }
            rel2 = rel2->next;
        }
        
        rel1 = rel1->next;
    }
    
    return commonFriends;
}

// Ortak arkadas analizi
void findCommonFriends(int userId1, int userId2) {
    int count;
    int* commonFriends = getCommonFriends(userId1, userId2, &count);
    int i;
    
    printf("Kullanici %d ve %d icin ortak arkadaslar:\n", userId1, userId2);
    if (count == 0) {
        printf("Ortak arkadas bulunamadi.\n");
    } else {
        for (i = 0; i < count; i++) {
            printf("Ortak Arkadas: %d\n", commonFriends[i]);
        }
    }
}

// Topluluk tespiti DFS yardimcisi
void dfsCommunity(int userId, int communityId, int* visited, int* community) {
    visited[userId] = 1;
    community[userId] = communityId;
    
    User* user = getUserById(userId);
    if (user == NULL) return;
    
    Relationship* current = user->relationships;
    while (current != NULL) {
        if (!visited[current->friendId]) {
            dfsCommunity(current->friendId, communityId, visited, community);
        }
        current = current->next;
    }
}

// Topluluk tespiti
void detectCommunities() {
    int visited[MAX_USERS] = {0};
    int community[MAX_USERS] = {0};
    int communityCount = 0;
    int i, c;
    
    // Her kullanici icin
    for (i = 0; i < userCount; i++) {
        int userId = users[i]->id;
        if (!visited[userId]) {
            communityCount++;
            dfsCommunity(userId, communityCount, visited, community);
        }
    }
    
    printf("Toplam %d topluluk tespit edildi:\n", communityCount);
    for (c = 1; c <= communityCount; c++) {
        printf("Topluluk %d: ", c);
        for (i = 0; i < userCount; i++) {
            int userId = users[i]->id;
            if (community[userId] == c) {
                printf("%d ", userId);
            }
        }
        printf("\n");
    }
}

// Etki alani hesaplama yardimcisi
int getInfluenceScore(int userId, int depth, int* visited) {
    if (visited[userId] || depth <= 0) return 0;
    
    visited[userId] = 1;
    
    User* user = getUserById(userId);
    if (user == NULL) {
        visited[userId] = 0;
        return 0;
    }
    
    int score = 1; // Kendisi
    
    Relationship* current = user->relationships;
    while (current != NULL) {
        if (!visited[current->friendId]) {
            score += getInfluenceScore(current->friendId, depth - 1, visited) / depth;
        }
        current = current->next;
    }
    
    visited[userId] = 0; // Bu dal icin ziyareti kaldir
    
    return score;
}

// Etki alani hesaplama
void calculateInfluence(int userId) {
    int visited[MAX_USERS] = {0};
    int score = getInfluenceScore(userId, 3, visited); // 3 seviye derinlik
    printf("Kullanici %d'nin etki skoru: %d\n", userId, score);
}

// Iliski agacini yazdir
void printRelationshipTree() {
    int i;
    for (i = 0; i < userCount; i++) {
        User* user = users[i];
        printf("Kullanici %d -> ", user->id);
        
        Relationship* current = user->relationships;
        if (current == NULL) {
            printf("Arkadas yok");
        } else {
            while (current != NULL) {
                printf("%d ", current->friendId);
                current = current->next;
            }
        }
        printf("\n");
    }
}

// Kullanicinin iliskilerini yazdir
void printUserRelationships(User* user) {
    if (user == NULL) {
        printf("Kullanici bulunamadi\n");
        return;
    }
    
    printf("Kullanici %d'nin Arkadaslari: ", user->id);
    Relationship* current = user->relationships;
    if (current == NULL) {
        printf("Arkadas yok\n");
    } else {
        while (current != NULL) {
            printf("%d ", current->friendId);
            current = current->next;
        }
        printf("\n");
    }
}

// Ana fonksiyon
int main() {
    // NIL dugumunu baslat
    NIL = (RBNode*)malloc(sizeof(RBNode));
    if (NIL == NULL) {
        printf("Bellek hatasi: NIL dugumu olusturulamadi\n");
        exit(1);
    }
    NIL->color = BLACK;
    NIL->left = NULL;
    NIL->right = NULL;
    NIL->parent = NULL;
    rbRoot = NIL;

    // Veri dosyasini yukle
    loadDataFromFile("veriseti.txt");
    
    printf("Sosyal Ag Grafigi basariyla yuklendi!\n");
    
    // Iliski agacini yazdir
    printf("\n1. Iliski Agaci:\n");
    printRelationshipTree();
    
    // DFS ile belirli mesafedeki arkadaslari bul
    printf("\n2. DFS ile Arkadas Arama (Kullanici 101, Mesafe 2):\n");
    dfsSearch(101, 2);
    
    // Ortak arkadas analizi
    printf("\n3. Ortak Arkadas Analizi (Kullanici 101 ve 104):\n");
    findCommonFriends(101, 104);
    
    // Topluluk tespiti
    printf("\n4. Topluluk Tespiti:\n");
    detectCommunities();
    
    // Etki alani hesaplama
    printf("\n5. Etki Alani Hesaplama (Kullanici 101):\n");
    calculateInfluence(101);
    
    // Kirmizi-Siyah Agac testi
    printf("\n6. Kirmizi-Siyah Agac ile Kullanici Arama:\n");
    RBNode* foundNode = rbSearch(rbRoot, 103);
    if (foundNode != NIL) {
        printf("Kullanici 103 bulundu!\n");
        printf("Iliskileri: ");
        printUserRelationships(foundNode->user);
    } else {
        printf("Kullanici bulunamadi.\n");
    }
    
    // Bellegi temizle
    int i;
    for (i = 0; i < userCount; i++) {
        Relationship* current = users[i]->relationships;
        while (current != NULL) {
            Relationship* temp = current;
            current = current->next;
            free(temp);
        }
        free(users[i]);
    }
    
    // Kirmizi-Siyah Agac bellegini temizle
    if (rbRoot != NIL) {
        freeRBTree(rbRoot);
    }
    free(NIL);
    
    return 0;
}
