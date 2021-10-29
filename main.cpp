#include <iostream>
#include <new>

template <typename T> class Lista {
private:
    void operator=(const Lista &);
    Lista(const Lista &);

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() = 0;
    virtual int &trenutni() const = 0;
    virtual int &trenutni()  = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T &el) = 0;
    virtual void dodajIza(const T &el) = 0;
    virtual T &operator[](int broj) = 0;
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
    int brojElemenata() override { return duzina; }
    int &trenutni() override { if(duzina == 0) throw std::domain_error("Lista prazna! ");return *(l[tekuci]); }
    int &trenutni()const override { if(duzina == 0) throw std::domain_error("Lista prazna! "); return *(l[tekuci]);  }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T &el) override;
    void dodajIza(const T &el) override;
    T &operator[](int broj) override { return *l[broj]; };
    T const &operator[](int broj) const { return *l[broj]; };
    void prosiriKapacitet(T **pT);
};

template <typename T> NizLista<T>::NizLista(int size) {
    kapacitet =size;
    l = new T*[kapacitet];
}

template <typename T> bool NizLista<T>::prethodni() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    if (tekuci == 0)
        return false;
    else
        tekuci--;
    return true;
}

template <typename T> bool NizLista<T>::sljedeci() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    if (tekuci == duzina-1)
        return false;
    else
        tekuci++;
    return true;
}

template <typename T> void NizLista<T>::pocetak() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    tekuci = 0;
}
template <typename T> void NizLista<T>::kraj() {
    if(duzina == 0) throw std::domain_error("Lista prazna! ");
    tekuci = duzina - 1;
}

template <typename T> void NizLista<T>::obrisi() {
    if (duzina == 0)
        return;
    else if (tekuci == duzina - 1){
        delete l[tekuci];
        tekuci--;
    }
    else{
        delete l[tekuci];
        for (int i = tekuci; i < duzina; i++)
            l[i] = l[i+1];
    }
    duzina--;
    if(duzina == 0){
        delete[] l;
        l = nullptr;
    }
}
template <typename T>
void ispisiElemente (T *pokazivac, int duzina){
    std::cout<< "Elementi : ";
    for (int i = 0; i < duzina; ++i) {
        std::cout << *pokazivac[i] << " ";
    }
    std::cout << std::endl;
}


template <typename T> void NizLista<T>::dodajIspred(const T &el) {
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



template <typename T> void NizLista<T>::dodajIza(const T &el) {
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


int main() {

}
