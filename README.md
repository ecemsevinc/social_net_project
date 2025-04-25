<h1>Sosyal Ağ Analizi Projesi</h1>
<p>Bu proje, sosyal ağları temsil etmek ve analiz etmek için çeşitli ağaç tabanlı veri yapılarını kullanan kapsamlı bir C uygulamasıdır. Sosyal ağdaki kullanıcılar arası ilişkiler bir graf olarak modellenirken verimli arama ve analiz işlemleri için çeşitli veri yapıları kullanılmıştır.</p>
<h2>Proje Bileşenleri</h2>
<h3>Veri Yapıları</h3>
<ul> 
<li> <b>Kullanıcı (User):</b> ID'ye sahip ve ilişki listesini barındıran temel kullanıcı yapısı. </li>
<li> <b>İlişki (Relationship):</b> Bağlantılı liste yapısıyla bir kullanıcının arkadaşlarını temsil eder. </li>
<li> <b>Kırmızı-Siyah Ağaç (Red-Black Tree): </b> Kullanıcıları ID'lerine göre dengeli ve verimli bir şekilde depolamak için kullanılan gelişmiş ağaç yapısı. </li>
</ul>

<h2>Temel Özellikler </h2>
<h3>1.Veri Yükleme</h3>
<ul>
<li>Kullanıcı ve arkadaşlık ilişkileri verilerini "veriseti.txt" dosyasından okur.</li>
</ul>
<h3>2. İlişki Ağacı Oluşturma </h3>
<ul>
<li> Kullanıcılar arasındaki bağlantıları temsil etmek için komşuluk listesi yapısı kullanılmıştır. </li>
<li> Her kullanıcı için bağlı olduğu diğer kullanıcılar özel bir ilişki yapısında saklanır. </li>
<li> "printRelationshipTree() "fonksiyonu tüm kullanıcıların ilişki ağacını gösterir </li>
</ul>

<h3>3. DFS ile Belirli Mesafedeki Arkadaşları Bulma </h3>
<ul>
<li> Derinlik öncelikli arama (DFS) algoritması ile bir kullanıcıdan belirli bir mesafedeki tüm arkadaşlar bulunur.</li>
<li> <code>dfsSearch()</code> ve <code>dfsHelper()</code>getInfluenceScore()</code> fonksiyonları, belirli bir mesafe limiti dahilinde sosyal ağ grafiğini dolaşır. </li>
<li> Bu özellik "arkadaşların arkadaşları" gibi genişletilmiş sosyal ağ bağlantılarını keşfetmeye olanak tanır </li>
</ul>

<h3>4. Ortak Arkadaş Analizi </h3>
<ul>
<li> İki kullanıcı arasındaki ortak arkadaşları tespit eder </li>
<li> <code>findCommonFriends()</code> ve <code>getCommonFriends()</code> fonksiyonları iki kullanıcının bağlantı listelerini karşılaştırır. </li>
<li> Bu fonksiyon, sosyal ağlarda sıklıkla kullanılan "Tanıyor olabileceğin kişiler" özelliğinin temelidir. </li>
</ul>

<h3>5. Topluluk Tespiti </h3>
<ul>
<li> Bağlantılı bileşenleri (connected components) bularak sosyal ağ içindeki toplulukları tespit eder. </li>
<li> <code>detectCommunities()</code> ve <code>dfsCommunity()</code> fonksiyonları DFS kullanarak birbirine bağlı kullanıcı gruplarını belirler. </li>
<li> Her topluluk etiketlenir ve üyeleri listelenir. </li>
</ul>

<h3>6. Etki Alanı Hesaplama </h3>
<ul>
<li> Bir kullanıcının ağ üzerindeki etkisini hesaplar. </li>
<li> <code>calculateInfluence()</code> ve <code>getInfluenceScore()</code> fonksiyonları recursive olarak bir kullanıcının ne kadar geniş bir etki alanına sahip olduğunu belirler. </li>
<li> Etki skoru, kullanıcının doğrudan ve dolaylı bağlantılarının sayısına ve mesafesine göre hesaplanır. </li>
</ul>

<h3>7. Kırmızı-Siyah Ağaç Uygulaması </h3>
<ul>
<li> Kullanıcı ID'lerine göre dengeli ve verimli bir şekilde organizasyon sağlar </li>
<li> Bu veri yapısı, büyük sosyal ağlar için önemli olan hızlı kullanıcı arama işlemlerini optimize eder. </li>
<li> <code>rbInsert()</code>, <code>rbSearch()</code>, <code>leftRotate()</code>, <code>rightRotate()</code> ve <code>rbInsertFixup()</code> gibi fonksiyonlarla eksiksiz bir kırmızı-siyah ağaç implementasyonu içerir. </li>
</ul>

<h2> Veri Yapısı Seçimi ve Algoritmalar </h2>
<ul>
<li> <b>Kırmızı-Siyah Ağaç: </b> Kullanıcı aramaları için O(log n) karmaşıklık sağlayan dengeli bir ikili arama ağacıdır. Bu, büyük sosyal ağlarda kullanıcıların hızlı bir şekilde bulunmasını sağlar.</li>
<li> <b>Komşuluk Listesi: </b> Her kullanıcının arkadaşları için bağlı liste yapısı kullanılmıştır. Bu, bellek kullanımını optimize ederken, yeni arkadaşlık bağlantılarının O(1) sürede eklenmesine olanak tanır.</li>
<li>DFS (Derinlik Öncelikli Arama): </b> Sosyal ağ grafiğinde dolaşmak için kullanılır. Bu algoritma, belirli bir mesafedeki arkadaşları bulma, topluluk tespiti ve etki alanı hesaplama gibi çeşitli analizler için temel oluşturur. </li>
</ul>

<h2> Dosya Yapısı </h2>
<ul>
<li> <b> Ana Program: </b><code>main()</code>  fonksiyonu, sosyal ağın yüklenmesi ve çeşitli analiz fonksiyonlarının çalıştırılmasından sorumludur.Ana fonksiyon,programın akışını kontrol eder.Program akışı şu şekildedir: <ol>
<li> Veri Yükleme </li>
<li> İlişki ağacını görselleştirme </li>
<li> DFS ile arkadaş arama </li>
<li> Ortak arkadaş analizi </li>
<li> Topluluk tespiti </li>
<li> Etki alanı hesaplama </li>
<li> Kırmızı-siyah ağaç ile kullanıcı arama </li> </ol>  </li>

<li> <b> Veri Yükleme: </b> <code>loadDataFromFile()</code> fonksiyonu "veriseti.txt" dosyasından kullanıcı ve ilişki verilerini okur. </li>
<li> <b> Kullanıcı Yönetimi: </b> "createUser()", "getUserById()" gibi fonksiyonlar kullanıcıları yönetir. </li>
<li> <b> İlişki Yönetimi: </b> "addRelationship()","printUserRelationships()" ibi fonksiyonlar arkadaşlık ilişkilerini yönetir.</li>
<li> <b> Kırmızı-Siyah Ağaç: </b> <code>rbInsert()</code> ,<code>rbSearch()</code> gibi fonksiyonlar dengeli ağaç yapısını yönetir. </li>
<li> <b> Analiz Fonksiyonları: </b>  DFS, ortak arkadaş analizi, topluluk tespiti ve etki alanı hesaplamak için kullanılan fonksiyonlar. </li>
</li>

<h2>Örnek Veri Dosyası</h2>
<p>USER 101<br>
USER 102<br>
USER 103<br>
USER 104<br>
FRIEND 101 102 <br>
FRIEND 101 103<br>
FRIEND 102 104<br>
FRIEND 103 104<br>
FRIEND 104 105 <br>

  
<h2> Kullanım </h2>
<p> Programı çalıştırmak için: 
<ol>
<li> veriseti.txt dosyasını aşağıdaki formatta hazırlayın: <br>
USER <id> <br> 
FRIEND <id1> <id2> </li>
<li> Kodu derleyin: <br>
gcc -o sosyal_ag sosyal_ag.c </li>
<li> Programı çalıştırın: <br> 
./sosyal_ag </li>
</ol> </p>

<h2> Program Çıktısı</h2>
<p> Program çalıştırıldığında şu adımları gerçekleştirir: <br>
<ol>
<li>İlişki ağacını listeler. </li>
<li>Belirli bir kullanıcıdan belirli mesafedeki arkadaşları gösterir. </li>
<li>İki kullanıcı arasındaki ortak arkadaşları listeler. </li>
<li>Sosyal ağdaki toplulukları tespit eder ve listeler.</li>
<li>Belirli bir kullanıcının etki alanını hesaplar. </li>
<li>Kırmızı-Siyah ağaç kullanarak bir kullanıcıyı arar ve ilişkilerini gösterir. </li>
</ol> </p>
<p><b>Örnek program çıktısı:</b><br>
Sosyal Ag Grafigi basariyla yuklendi! <br>
1. Iliski Agaci:<br>
Kullanici 101 -> 103 102<br>
Kullanici 102 -> 104 101<br>
Kullanici 103 -> 104 101<br>
Kullanici 104 -> 105 103 102<br>
Kullanici 105 -> 104<br>
2. DFS ile Arkadas Arama (Kullanici 101, Mesafe 2):<br>
Kullanici 101'den en fazla 2 mesafedeki arkadaslar:<br>
Kullanici 103 (Mesafe: 1)<br>
Kullanici 104 (Mesafe: 2)<br>
Kullanici 102 (Mesafe: 1)<br>
3. Ortak Arkadas Analizi (Kullanici 101 ve 104):<br>
Kullanici 101 ve 104 icin ortak arkadaslar:<br>
Ortak Arkadas: 103<br>
Ortak Arkadas: 102<br>
4. Topluluk Tespiti:<br>
Toplam 1 topluluk tespit edildi:<br>
Topluluk 1: 101 102 103 104 105<br>
5. Etki Alani Hesaplama (Kullanici 101):<br>
Kullanici 101'nin etki skoru: 1<br>
6. Kirmizi-Siyah Agac ile Kullanici Arama:<br>
Kullanici 103 bulundu!<br>
Iliskileri: Kullanici 103'nin Arkadaslari: 104 101<br>
</p>


<h2> Lisans </h2>
<p>Bu proje MIT lisansı altında lisanslanmıştır. Daha fazla bilgi için LICENSE dosyasını inceleyebilirsiniz.</p>
