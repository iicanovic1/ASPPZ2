#include <iostream>
#include <new>

template <typename T> class Lista {
private:
    void operator=(const Lista &);
    Lista(const Lista &);

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual T trenutni() const = 0;
    virtual T &trenutni()  = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T &el) = 0;
    virtual void dodajIza(const T &el) = 0;
    virtual T &operator[](int broj) = 0;
    virtual T operator[](int broj) const = 0;
};

template <typename T> class NizLista : public Lista<T> {
private:
    unsigned int kapacitet ;
    int duzina = 0;
    int tekuci = 0;
    T **l = nullptr;

public:
    explicit  NizLista(int size = 50);
    ~NizLista()override {
        for(int i = 0; i < duzina; i++) delete l[i];
        delete []l;
    }
    int brojElemenata()const  override { return duzina; }
    T &trenutni() override { if(duzina == 0) throw std::domain_error("Lista prazna! ");return *(l[tekuci]); }
    T trenutni()const override { if(duzina == 0) throw std::domain_error("Lista prazna! "); return *(l[tekuci]);  }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T &el) override;
    void dodajIza(const T &el) override;
    T &operator[](int broj) override { return *l[broj]; };
    T operator[](int broj) const override{ return *l[broj]; };
    void prosiriKapacitet(T **pT);
    NizLista<T> &operator = (const NizLista<T> &lista);
    NizLista<T> &operator = (NizLista<T> &&lista);
    NizLista(const NizLista<T> &lista);
    NizLista(NizLista<T> &&lista);

};



template <typename T>
NizLista<T>::NizLista(int size) {
    kapacitet =size;
    l = new T*[kapacitet];
}

template <typename T>
bool NizLista<T>::prethodni() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    if (tekuci == 0)
        return false;
    else
        tekuci--;
    return true;
}

template <typename T>
bool NizLista<T>::sljedeci() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    if (tekuci == duzina-1)
        return false;
    else
        tekuci++;
    return true;
}

template <typename T>
void NizLista<T>::pocetak() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    tekuci = 0;
}

template <typename T>
void NizLista<T>::kraj() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    tekuci = duzina - 1;
}

template <typename T>
void NizLista<T>::obrisi() {
    if (duzina == 0)
        return;
    else if (tekuci == duzina - 1){
        delete l[tekuci];
        if (tekuci != 0)tekuci--;
    }
    else{
        delete l[tekuci];
        for (int i = tekuci; i < duzina; i++)
            l[i] = l[i+1];
    }
    duzina--;
    /*if(duzina == 0){
        delete[] l;
        l = nullptr;
    }*/
}

template <typename T>
void ispisiElemente (T *pokazivac, int duzina){
    std::cout<< "Elementi : ";
    for (int i = 0; i < duzina; ++i) {
        std::cout << *pokazivac[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void NizLista<T>::dodajIspred(const T &el) {
    if (duzina == 0) {
        l[0] = new T (el);
        duzina++;
    }
    else if (duzina < kapacitet) {

        for (int i = duzina-1; i >= tekuci; i--) {
            l[i+1] = l[i];
        }
        l[tekuci] = new T (el);
        tekuci++;
        duzina++;
    } else if (duzina == kapacitet) {
        prosiriKapacitet(l);
        this->dodajIspred(el);
    }
    //ispisiElemente(l,duzina);
}

template <typename T>
void NizLista<T>::dodajIza(const T &el) {
    if (duzina == 0) {
        l[0] = new T (el);
        duzina++;
        tekuci =0;
    }
    else if (duzina < kapacitet ) {

        for (int i = duzina-1; i > tekuci; i--) {
            l[i+1] = l[i];
        }
        l[tekuci + 1] = new T (el);
        duzina++;
    } else if (duzina == kapacitet)
    {
        prosiriKapacitet(l);
        this->dodajIza(el);
    }
    //ispisiElemente(l,duzina);
}

template<typename T>
void NizLista<T>::prosiriKapacitet(T **pT) {
    kapacitet = kapacitet*2;
    T ** lista = l;
    l = new T* [kapacitet];
    for (int i = 0; i < duzina ; i++)
        l[i] = lista[i];

    delete []lista;

}

template<typename T>
NizLista<T> &NizLista<T>::operator=(const NizLista<T> &lista)  {
    if(&lista != this) { // Samododjela?
        for(int i = 0; i < duzina; i++) delete l[i];
        delete []l;
        kapacitet = lista.kapacitet;
        duzina = lista.duzina;
        tekuci = lista.tekuci;
        l = new T*[kapacitet];
        for (int i = 0; i < duzina ; i++)
            l[i] = new T(*lista.l[i]);
    }
    return *this;
}

template<typename T>
NizLista<T> &NizLista<T>::operator=(NizLista<T> &&lista) {
    if(&lista != this) { // Samododjela?
        for(int i = 0; i < duzina; i++) delete l[i];
        delete []l;
        l = lista.l;
        kapacitet = lista.kapacitet;
        duzina = lista.duzina;
        tekuci = lista.tekuci;
        lista.l = nullptr;
    }
    return *this;
}

template<typename T>
NizLista<T>::NizLista(const NizLista<T> &lista) {
    kapacitet = lista.kapacitet;
    duzina = lista.duzina;
    tekuci = lista.tekuci;
    l = new T*[kapacitet];
    for (int i = 0; i < duzina ; i++)
        l[i] = new T(*lista.l[i]);

}

template<typename T>
NizLista<T>::NizLista(NizLista<T> &&lista) {
    kapacitet = lista.kapacitet;
    duzina = lista.duzina;
    tekuci = lista.tekuci;
    l = lista.l;
    lista.l = nullptr;
}

template <typename T>
struct Cvor{
    T element;
    Cvor *sljedeci;
    Cvor (const T &element, Cvor *sljedeci ): element(element),sljedeci(sljedeci){}
    Cvor(Cvor *sljedeci = nullptr) : sljedeci(sljedeci){}
    Cvor(const Cvor &cvor){ element=cvor.element; sljedeci=cvor.sljedeci;}
};

template <typename T> class JednostrukaLista : public Lista<T> {
private:
    Cvor<T> *pocetakListe = nullptr;
    Cvor<T> *krajListe = nullptr;
    Cvor<T> *tekuci = nullptr;
    Cvor<T> *prije_tekuci = nullptr;
    int lduzina{};
    int dduzina{};
    void Iniciraj();
    void Unisti();
public:
    explicit  JednostrukaLista(){ Iniciraj();} // radi
    ~JednostrukaLista()override { Unisti(); } // vjerovati ćemo da radi pokazati će testovi
    int brojElemenata() const override  { return dduzina+lduzina; } // radi
    T &trenutni() override{return tekuci->element; }; // radi
    T trenutni()const override{return tekuci->element; }; // radi
    bool prethodni() override; // radi
    bool sljedeci() override{
        if (tekuci == krajListe) return false;
        prije_tekuci = prije_tekuci->sljedeci; tekuci = tekuci->sljedeci; return true;
    }; //radi
    void pocetak() override{ tekuci = pocetakListe->sljedeci; prije_tekuci = pocetakListe;}; //radi
    void kraj() override{ tekuci = krajListe;}; //radi
    void obrisi() override; //radi
    void dodajIspred(const T &el) override; // radi
    void dodajIza(const T &el) override; //radi
    T &operator[](int broj) override;
    T operator[](int broj) const override;
    JednostrukaLista<T> &operator = (const JednostrukaLista<T> &lista);
    JednostrukaLista<T> &operator = (JednostrukaLista<T> &&lista);
    JednostrukaLista(const JednostrukaLista<T> &lista){
        pocetakListe=new Cvor<T>(*lista.pocetakListe);
        if(pocetakListe == nullptr) return;
        Cvor<T> *tempNova=pocetakListe;
        Cvor<T> *tempStara=lista.pocetakListe;
        while(tempStara->sljedeci != nullptr) {
            tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
            tempNova=tempNova->sljedeci;
            tempStara=tempStara->sljedeci;
        }
        dduzina=lista.dduzina;
        lduzina = lista.lduzina;
    }; // radi
    JednostrukaLista(JednostrukaLista<T> &&lista);
    template<typename T1>
    friend void ispisiJednostruka ( JednostrukaLista<T1> lista);

};

template<typename T>
void ispisiJednostruka ( JednostrukaLista<T> lista){
    std::cout << "Podaci liste ispod " << std::endl <<
                 "duzina : " << lista.brojElemenata() <<std::endl ;
    if(lista.brojElemenata() == 0) {
        std::cout << "Prazna lista nista za prikazati" << std::endl;
        throw std::domain_error("Prazna lista nista za prikazati");
    }
    std::cout <<  "Elementi :" << std::endl;
    Cvor<T> * privremeni = lista.pocetakListe->sljedeci;
    while (privremeni != nullptr){
        std::cout << privremeni->element<<" ";
        privremeni = privremeni->sljedeci;
    }
}


template<typename T>
void JednostrukaLista<T>::Iniciraj() {
    tekuci = krajListe = pocetakListe = new Cvor<T>;
    dduzina = lduzina = 0;
}

template<typename T>
void JednostrukaLista<T>::Unisti() {
    while (pocetakListe != nullptr){
        tekuci = pocetakListe;
        pocetakListe = pocetakListe->sljedeci;
        delete tekuci;
    }
    pocetakListe = krajListe = tekuci = nullptr;
}

template<typename T>
void JednostrukaLista<T>::dodajIspred(const T &el) {
    if(this->brojElemenata() == 0) {
        //lista  ima samo fiktivni čvor i treba povezati ga sa novim elemetnom
        pocetakListe->sljedeci = new Cvor<T>(el, nullptr);
        tekuci = pocetakListe->sljedeci;
        prije_tekuci = pocetakListe;
        krajListe = tekuci;
        //std::cout << "Prije tekuci : " << prije_tekuci->element << " a tekuci : " << tekuci->element<< std::endl;
        lduzina--;
        dduzina++;
    }else{
        //std::cout << "Prije tekuci : " << prije_tekuci->element << " a tekuci : " << tekuci->element<< std::endl;
        prije_tekuci->sljedeci = new Cvor<T>(el,tekuci);
        prije_tekuci = prije_tekuci->sljedeci;
    }
    lduzina++;
}

template<typename T>
void JednostrukaLista<T>::dodajIza(const T &el) {
    if(this->brojElemenata() == 0) {
        //lista  ima samo fiktivni čvor i treba povezati ga sa novim elemetnom
        pocetakListe->sljedeci = new Cvor<T>(el, nullptr);
        tekuci = pocetakListe->sljedeci;
        prije_tekuci = pocetakListe;
        krajListe = tekuci;
        //std::cout << "kraj : " << krajListe->element << " a tekuci : " << tekuci->element<< std::endl;
    }
    else{
        Cvor<T> * temp = tekuci->sljedeci;
        tekuci->sljedeci = new Cvor<T>(el,temp);
        if (krajListe == tekuci) krajListe = tekuci->sljedeci;
        //std::cout << "kraj : " << krajListe->element << " a tekuci : " << tekuci->element<< std::endl;
    }
    dduzina++;
}

template<typename T>
bool JednostrukaLista<T>::prethodni() {
    if (brojElemenata() == 0 || brojElemenata() == 1) return false;
    Cvor<T> *prijeTemp = pocetakListe;
    Cvor<T> *temp = prijeTemp->sljedeci;
    while (temp->sljedeci != tekuci){
        temp = temp->sljedeci;
        prijeTemp  = prijeTemp->sljedeci;
    }
    prije_tekuci = prijeTemp;
    tekuci = temp;
    return true;
}

template<typename T>
void JednostrukaLista<T>::obrisi() {
    if(brojElemenata() == 0) return;
    if(this->brojElemenata() == 1){
        delete tekuci;
        krajListe = pocetakListe;
        tekuci = pocetakListe;
        prije_tekuci =nullptr;
        pocetakListe->sljedeci = nullptr;
    }else if(tekuci == krajListe){
        this->prethodni();
        krajListe = tekuci;
        Cvor<T> *brisanje = tekuci->sljedeci;
        delete brisanje;
        tekuci->sljedeci = nullptr;
    }else {
        prije_tekuci->sljedeci = tekuci->sljedeci;
        //std::cout<< " brisem " << tekuci->element << " a prije mene je " <<prije_tekuci->element;
        delete tekuci;
        tekuci = prije_tekuci->sljedeci;
    }
dduzina--;
}

template<typename T>
T &JednostrukaLista<T>::operator[](int broj) {
    Cvor<T> *temp=pocetakListe->sljedeci;
    for(int j=0; j<broj; j++) temp=temp->sljedeci;
    return temp->element;
}

template<typename T>
T JednostrukaLista<T>::operator[](int broj) const {
    Cvor<T> *temp=pocetakListe->sljedeci;
    for(int j=0; j<broj; j++) temp=temp->sljedeci;
    return temp->element;
}

template<typename T>
JednostrukaLista<T> &JednostrukaLista<T>::operator=(const JednostrukaLista<T> &lista) {
    this->Unisti();
    pocetakListe=new Cvor<T>(*lista.pocetakListe);
    if(pocetakListe == nullptr) return *this;
    Cvor<T> *tempNova=pocetakListe;
    Cvor<T> *tempStara=lista.pocetakListe;
    while(tempStara->sljedeci != nullptr) {
        tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
        tempNova=tempNova->sljedeci;
        tempStara=tempStara->sljedeci;
    }
    dduzina=lista.dduzina;
    lduzina = lista.lduzina;
    return *this;
}

template<typename T>
JednostrukaLista<T> &JednostrukaLista<T>::operator=(JednostrukaLista<T> &&lista) {
    if(&lista==this) return *this;
    this->Unisti();
    pocetakListe = lista.pocetakListe;
    tekuci = lista.tekuci;
    prije_tekuci = lista.prije_tekuci;
    krajListe = lista.krajListe;
    dduzina = lista.dduzina;
    lduzina = lista.lduzina;
    pocetakListe = nullptr;
    krajListe= nullptr;
    tekuci = nullptr;
    prije_tekuci = nullptr;
    return *this;
}

template<typename T>
JednostrukaLista<T>::JednostrukaLista(JednostrukaLista<T> &&lista) {
    pocetakListe = lista.pocetakListe;
    tekuci = lista.tekuci;
    prije_tekuci = lista.prije_tekuci;
    krajListe = lista.krajListe;
    dduzina = lista.dduzina;
    lduzina = lista.lduzina;
    pocetakListe = nullptr;
    krajListe= nullptr;
    tekuci = nullptr;
    prije_tekuci = nullptr;
}

//testne funkcije

template<typename T>
bool testTrenutniPrethodniSljedeci (){
    JednostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(5);
    testnaLista.sljedeci();
    testnaLista.trenutni() = 50 ;
    testnaLista.prethodni();
    testnaLista.trenutni() = 10 ;
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(2);

    ispisiJednostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testPocetakKrajObrisi (){
    JednostrukaLista<int> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.obrisi();
    ispisiJednostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testOperatorDodjele (){
    JednostrukaLista<T> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.obrisi();
    ispisiJednostruka<int>(testnaLista);
    return true;
}


template<typename T>
bool testKopirajucikonstruktor (){
    JednostrukaLista<T> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.obrisi();
    ispisiJednostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testRVrijednosnaReferenca (){
    JednostrukaLista<T> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.obrisi();
    ispisiJednostruka<int>(testnaLista);
    return true;
}
template<typename T>
bool testKonstruktoraJednostruka (){
    JednostrukaLista<int> testnaLista ;
    ispisiJednostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testDodajIspredJednostruka (){
    JednostrukaLista<int> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.dodajIspred(2);
    testnaLista.dodajIspred(3);
    testnaLista.dodajIspred(4);
    testnaLista.dodajIspred(5);
    ispisiJednostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testDodajIzaJednostruka (){
    JednostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    ispisiJednostruka<int>(testnaLista);
    return true;
}






int main() {
    testKonstruktoraJednostruka<int>();
    testDodajIzaJednostruka<int>();
    testDodajIspredJednostruka<int>();
    testTrenutniPrethodniSljedeci<int>();
    testPocetakKrajObrisi<int>();
    testRVrijednosnaReferenca<int>();
    testKopirajucikonstruktor<int>();
    testOperatorDodjele<int>();
    testDodajIspredJednostruka<int>();
    testTrenutniPrethodniSljedeci<int>();
    testPocetakKrajObrisi<int>();
    testTrenutniPrethodniSljedeci<int>();
    testPocetakKrajObrisi<int>();
    testRVrijednosnaReferenca<int>();
    testKopirajucikonstruktor<int>();


}
