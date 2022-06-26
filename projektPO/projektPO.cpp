#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

typedef bool _forma;   //forma w jakiej przepisano lek
typedef int _dawka;    //dawkowanie leku
typedef string _data;  //dd.mm.rrrr
typedef string _pesel; //pesel
typedef float _cena;   //cena zł -,--
typedef int _id;       //identyfikator (np zakupu)

template <typename T > void wypisz(T zmienna) //szablon funkcji 
{
    cout << zmienna;
}

class Osoba //ogolna klasa dla osob uczestniczacych w tranzakcji
{
protected:
    string imie;
    string nazwisko;
    _data data_urodzenia;
    _pesel pesel;
public:
    virtual void wypisz_informacje() //wypisanie informacji o osobie
    {
        /*cout << imie << endl;
        cout << nazwisko << endl;
        cout << data_urodzenia << endl;
        cout << pesel << endl;*/
        cout << imie << " ";
        cout << nazwisko << " ";
        cout << data_urodzenia << " ";
        cout << pesel << endl;
    }
    virtual void wypisz_imie_nazwisko() //wypisanie samego imienia i nazwiska persony
    {
        cout << imie << " " << nazwisko;
    }
};
class Lekarz : public Osoba //lekarz - wystawiajacy recepte
{
public: 
    friend class Recepta;
    Lekarz();
    ~Lekarz(){};
    Lekarz(string imie, string nazwisko, _data data_urodzenia, _pesel pesel)
    {
        this->imie = imie;
        this->nazwisko = nazwisko;
        this->data_urodzenia = data_urodzenia;
        this->pesel = pesel;
    }
    virtual void wypisz_informacje()
    {
        /*cout << imie << endl;
        cout << nazwisko << endl;
        cout << data_urodzenia << endl;
        cout << pesel << endl;*/
        cout << imie << " ";
        cout << nazwisko << " ";
        cout << data_urodzenia << " ";
        cout << pesel << endl;
    }
    virtual void wypisz_imie_nazwisko()
    {
        cout << imie << " " << nazwisko;
    }
};
class Aptekarz : public Osoba //aptekarz, pracownik apteki, realizujacy tranakcje, recepty
{
public:
    friend class Tranzakcja;
    friend class Recepta;
    Aptekarz();
    ~Aptekarz(){};
    Aptekarz(string imie, string nazwisko, _data data_urodzenia, _pesel pesel)
    {
        this->imie = imie;
        this->nazwisko = nazwisko;
        this->data_urodzenia = data_urodzenia;
        this->pesel = pesel;
    }
    virtual void wypisz_informacje()
    {
        /*cout << imie << endl;
        cout << nazwisko << endl;
        cout << data_urodzenia << endl;
        cout << pesel << endl;*/
        cout << imie << " ";
        cout << nazwisko << " ";
        cout << data_urodzenia << " ";
        cout << pesel << endl;
    }
    virtual void wypisz_imie_nazwisko()
    {
        cout << imie << " " << nazwisko;
    }
};
class Pacjent : public Osoba //pacjent - osoba dokonujaca zakupu w aptece
{
private:
    friend class Tranzakcja;
    friend class Recepta;
public:
    Pacjent();
    ~Pacjent(){};
    Pacjent(string imie, string nazwisko, _data data_urodzenia, _pesel pesel)
    {
        this->imie = imie;
        this->nazwisko = nazwisko;
        this->data_urodzenia = data_urodzenia;
        this->pesel = pesel;
    }
    virtual void wypisz_informacje()
    {
        /*cout << imie << endl;
        cout << nazwisko << endl;
        cout << data_urodzenia << endl;
        cout << pesel << endl;*/
        cout << imie << " ";
        cout << nazwisko << " ";
        cout << data_urodzenia << " ";
        cout << pesel << endl;
    }
    virtual void wypisz_imie_nazwisko()
    {
        cout << imie << " " << nazwisko;
    }
};
class Lek //lek sprzedawany w aptece
{
private:
    friend class Recepta;
    friend class Tranzakcja;    
public:
    string kategoria = " ";
    string nazwa;
    bool czy_recepta; //czy lek jest na recepre 0-nie, 1-tak
    _forma jaka_forma; //0- tabletka, 1- syrop
    _cena cena;
    Lek();
    ~Lek(){};
    Lek(string nazwa, _forma forma, _cena cena)
    {
        this->nazwa = nazwa;
        this->jaka_forma = forma;
        this->cena = cena;
    }
    Lek(string kategoria, string nazwa, _forma forma, _cena cena)
    {
        this->kategoria = kategoria;
        this->nazwa = nazwa;
        this->jaka_forma = forma;
        this->cena = cena;
    }
    Lek(string kategoria, string nazwa, _forma forma, _cena cena, bool czy_recepta)
    {
        this->kategoria = kategoria;
        this->nazwa = nazwa;
        this->jaka_forma = forma;
        this->cena = cena;
        this->czy_recepta = czy_recepta;
    }
    void wypisz_informacje()
    {
        if(kategoria != " ") cout << "Kategoria: " << this->kategoria;
        cout << " Nazwa: " << this->nazwa;
        cout << " Forma: ";
        if       (this->jaka_forma == 0) cout << "tabletki";
        else  if (this->jaka_forma == 1) cout << "syrop";
        cout << " Cena: " << this->cena << endl;
    }
};

class Recepta //recepta wystawiona przez lekarza na konkretnego pacjenta 
{
private:
    friend class Tranzakcja;
    _data data_wystawienia;
    _data data_realizacji;
    
public:
    Lekarz* lekarz;
    Pacjent* pacjent;
    vector<Lek> leki_na_recepcie;
    Recepta();
    Recepta(Lekarz *lekarz, Pacjent *pacjent, _data data_wystaw)
    {
        this->lekarz = lekarz;
        this->pacjent = pacjent;
        this->data_wystawienia = data_wystaw;
    };
    ~Recepta()
    {
        leki_na_recepcie.clear(); //kasuje elementy
        leki_na_recepcie.shrink_to_fit(); // w teorii powinno dac mu rozmiar 0 teraz, czyli zwolnic pamiec
        lekarz->~Lekarz();
        pacjent->~Pacjent();
    }
    void dodaj_lek_na_recepte(Lek lek)
    {
        leki_na_recepcie.push_back(lek);
    }
    
};

_cena operator+(_cena a, Lek& l) //operator ulatwiajacy sumowanie cen
{
    return a + l.cena;
}

class Tranzakcja //informacje nt. tranzakcji w aptece
{
private:
    friend class Archiwum;
    static _id licznik_zakupow;
    _id identyfikator_zakupu;
    vector<Lek> zakupione_leki;
public:
    _cena wartosc_zakupu = 0;
    Aptekarz* aptekarz;
    Recepta* recepta;
    Pacjent* pacjent;
    Tranzakcja();
    Tranzakcja(Aptekarz *aptekarz, Pacjent *pacjent)
    {
        this->aptekarz = aptekarz;
        this->pacjent = pacjent;
        this->wartosc_zakupu = 0;
        this->licznik_zakupow++;
        this->identyfikator_zakupu = licznik_zakupow;
    }
    Tranzakcja(Aptekarz *aptekarz, Recepta *recepta)
    {
        this->licznik_zakupow++;
        this->aptekarz = aptekarz;
        this->pacjent = recepta->pacjent;
        this->recepta = recepta;
        this->wartosc_zakupu = 0;
        this->identyfikator_zakupu = licznik_zakupow;
        dodaj_leki_z_recepty();
    }
    ~Tranzakcja()
    {
        //aptekarz->~Aptekarz();
        //recepta->~Recepta();
        //delete recepta;
        //pacjent->~Pacjent();
        zakupione_leki.clear();
        zakupione_leki.shrink_to_fit();
    }
    void dodaj_lek(Lek lek)
    {
        zakupione_leki.push_back(lek);
        this->wartosc_zakupu += lek.cena;
    }
    void dodaj_lek(string nazwa, _forma forma, _cena cena)
    {
        zakupione_leki.push_back(Lek(nazwa, forma, cena));
        this->wartosc_zakupu += cena;
    }
    void dodaj_lek(string kategoria, string nazwa, _forma forma, _cena cena)
    {
        Lek lek(kategoria, nazwa, forma, cena);
        zakupione_leki.push_back(lek);
        this->wartosc_zakupu += cena;
    }
    void dodaj_leki_z_recepty()
    {
        for (Lek lek : recepta->leki_na_recepcie )
        {
            zakupione_leki.push_back(lek);
            cout << lek.nazwa << endl;
            this->wartosc_zakupu = wartosc_zakupu + lek;
            //wartosc_zakupu += lek.cena;
        }
    }
    void wypisz_leki()
    {
        for (Lek lek : zakupione_leki )
        {
            lek.wypisz_informacje();
        }
    }
    void podaj_identyfikator_tranzakcji()
    {
        cout << "id:" << identyfikator_zakupu << endl;
    }
    _cena podaj_cene()
    {
        return this->wartosc_zakupu;
    }
};

class Archiwum 
{
public:
    vector<Tranzakcja>archiwum;
    int ilosc_tranzakcji_w_archiwum;
    Archiwum()
    {
        this->ilosc_tranzakcji_w_archiwum = 0;
    }
    ~Archiwum()
    {
        for (auto i : archiwum)
        {
            i.~Tranzakcja();
        }
        archiwum.clear();
        archiwum.shrink_to_fit();
    }
    void dodaj_do_archiwum(Tranzakcja* tranzakcja)
    {
        this->ilosc_tranzakcji_w_archiwum++;
        archiwum.push_back(*tranzakcja);
    }
   void wypisz_tranzakcje()
    {
        for (Tranzakcja tranzakcja : archiwum)
        {
            cout << "identyfikator tranzakcji: ";
            tranzakcja.podaj_identyfikator_tranzakcji();
            //cout <<"Kwota: " << tranzakcja.podaj_cene() << endl;
            wypisz("Kwota: "); wypisz(tranzakcja.podaj_cene()); cout << endl; //szablon funkcji 
            if (tranzakcja.recepta != nullptr)
            {
                cout << "Lekarz wystawiajacy recepte: " << endl;
                ((tranzakcja.recepta)->lekarz)->wypisz_informacje();
            }
            cout << "Aptekarz: " << endl;
            tranzakcja.aptekarz->wypisz_informacje();
            cout << "Kupujacy: " << endl;
            tranzakcja.pacjent->wypisz_informacje();
            cout << "Zakupione leki: " << endl;
            tranzakcja.wypisz_leki();
            cout << endl;
        }
    }
};
int Tranzakcja::licznik_zakupow = 0;


int main()
{
    Archiwum archiwum;
    Lekarz lekarz("Adam", "Janukowicz", "22.11.1978", "7811223212");
    Pacjent pacjent("Jan", "Kulczak", "17.09.1992", "92091702332");
    Recepta recepta(&lekarz, &pacjent, "21.11.2020");
    Recepta recepta2(new Lekarz("Aleksandra", "Szymczak", "01.06.2001", "01060112444"),new Pacjent("Dominik", "Rozycki", "22.08.2001", "01282202333"), "08.01.2022");
    recepta2.dodaj_lek_na_recepte(Lek("Przeciwbulowy", "Apap", 1, 39.99, 1));
    recepta2.dodaj_lek_na_recepte(Lek("Przeciwbulowy", "Ibuprofen", 0, 19.75, 1));
    recepta2.lekarz->wypisz_imie_nazwisko(); cout << endl;
    recepta2.pacjent->wypisz_informacje();

    Aptekarz aptekarz("Anna", "Gromska", "19.02.1997", "97221951242");
    Tranzakcja tranzakcja(new Aptekarz("Janusz", "krowin-Mikke", "01.01.1944", "44010112312"), &recepta2);
    tranzakcja.dodaj_lek("Rutinoskorwin", 1, 9.99);
    //tranzakcja.dodaj_leki_z_recepty();
    cout << tranzakcja.podaj_cene()<<endl;
    tranzakcja.podaj_identyfikator_tranzakcji();
    archiwum.dodaj_do_archiwum(&tranzakcja);
    Tranzakcja tranzakcja2(new Aptekarz("Adam", "Pedrele", "21.11.1998", "98112112312"), new Pacjent("Pawian", "Marcinski", "01.01.2001", "01010112312"));
    tranzakcja2.dodaj_lek("Paracetamol", 0, 19.99);
    archiwum.dodaj_do_archiwum(&tranzakcja2);
    cout << "Tranzakcji  w archiwum: ";
    cout << archiwum.ilosc_tranzakcji_w_archiwum << endl << endl;
    archiwum.wypisz_tranzakcje();
    //lekarz.wypisz_imie_nazwisko();
}
