#include <Eigen/Dense> // Wyznacznik
#include <iomanip>     // Do formatowania wynikow
#include <iostream>
#include <stdexcept> //Wyjatki
#include <vector>

class Matrix
{
private:
    std::vector< std::vector< double > > dane;             // G³owne dane macierzy, w postaci wektora 2D (macierzy)
    int                                  wiersze, kolumny; // Ilosc wierszy oraz kolumn

public:
    // Konstruktor domyslny - wszystkie wartosci na 0, ale trzeba wybrac liczbe kolumn
    Matrix(int wiersze1, int kolumny1, double wartosc_domyslna = 0.0)
        : wiersze(wiersze1), kolumny(kolumny1), dane(wiersze1, std::vector< double >(kolumny1, wartosc_domyslna))
    {}

    // Konstruktor z inicjalizacja
    Matrix(const std::vector< std::vector< double > >& wartosci) : wiersze(wartosci.size()), dane(wartosci)
    {
        if (wartosci.empty())
        {
            kolumny = 0;
        }
        else
        {
            kolumny = wartosci[0].size();
        }
        // Sprawdzenie, czy wszystkie wiersze maja tyle samo elementow (kolumn)
        for (const auto& wiersz : wartosci)
        {
            if (wiersz.size() != kolumny)
            {
                throw std::invalid_argument("Wszystkie wiersze musz¹ miec dokladnie tyle samo kolumn");
            }
        }
    }

    // Pobieranie liczby wierszy i kolumn
    int getWiersze() const { return wiersze; }
    int getKolumny() const { return kolumny; }

    // Dostep do elementow
    double& operator()(int wiersz, int kolumna) // Modyfikacja + odczyt
    {
        if (wiersz >= wiersze || kolumna >= kolumny)
            throw std::out_of_range("Indeks poza wymiarami macierzy");
        return dane[wiersz][kolumna];
    }

    double operator()(int wiersz, int kolumna) const // Tylko odczyt
    {
        if (wiersz >= wiersze || kolumna >= kolumny)
            throw std::out_of_range("Indeks poza wymiarami macierzy");
        return dane[wiersz][kolumna];
    }

    // Dodawanie macierzy
    Matrix operator+(const Matrix& druga_macierz) const // Druga macierz jako wskaznik do niej
    {
        if (wiersze != druga_macierz.wiersze || kolumny != druga_macierz.kolumny) // Wymiary
        {
            throw std::invalid_argument("Wymiary macierzy musza byc jednakowe");
        }

        Matrix wynik(wiersze, kolumny); // Obliczenie wyniku, postac macierzowa
        for (int i = 0; i < wiersze; ++i)
        {
            for (int j = 0; j < kolumny; ++j)
            {
                wynik(i, j) = dane[i][j] + druga_macierz(i, j);
            }
        }
        return wynik;
    }

    // Odejmowanie macierzy
    Matrix operator-(const Matrix& druga_macierz) const
    {
        if (wiersze != druga_macierz.wiersze || kolumny != druga_macierz.kolumny)
        {
            throw std::invalid_argument("Wymiary macierzy musza byc jednakowe");
        }

        Matrix wynik(wiersze, kolumny); // Obliczenie wyniku, postac macierzowa
        for (int i = 0; i < wiersze; ++i)
        {
            for (int j = 0; j < kolumny; ++j)
            {
                wynik(i, j) = dane[i][j] - druga_macierz(i, j);
            }
        }
        return wynik;
    }

    // Mno¿enie macierzy
    Matrix operator*(const Matrix& druga_macierz) const
    {
        if (kolumny != druga_macierz.wiersze)
        {
            throw std::invalid_argument(
                "Przy mnozeniu liczba kolumn macierzy 1 i wierszy macierzy 2 musi byc jednakowa");
        }

        Matrix wynik(wiersze, druga_macierz.kolumny); // Obliczenie wyniku, postac macierzowa
        for (int i = 0; i < wiersze; ++i)
        {
            for (int j = 0; j < druga_macierz.kolumny; ++j)
            {
                for (int k = 0; k < kolumny; ++k)
                {
                    wynik(i, j) += dane[i][k] * druga_macierz(k, j);
                }
            }
        }
        return wynik;
    }

    // Obliczanie wyznacznika przy u¿yciu biblioteki Eigen
    double wyznacznik() const
    {
        if (wiersze != kolumny)
        {
            throw std::invalid_argument("Wyznacznik tylko dla macierzy kwadratowej");
        }

        Eigen::MatrixXd eigenMatrix(wiersze, kolumny);
        for (int i = 0; i < wiersze; ++i)
        {
            for (int j = 0; j < kolumny; ++j)
            {
                eigenMatrix(i, j) = dane[i][j];
            }
        }

        return eigenMatrix.determinant();
    }

    // Wypisywanie macierzy, uzycie biblioteki iomanip
    void print() const
    {
        for (const auto& wiersz : dane)
        {
            for (double wartosc : wiersz)
            {
                std::cout << std::setw(8) << wartosc << " ";
            }
            std::cout << "\n";
        }
    }
    // Brak destruktora - Rule of 5
};

// Uzycie poszczegolnych metod
int main()
{
    try
    {
        // Deklaracja macierzy - podanie wektora w kodzie
        Matrix A({{3, 2, 3}, {4, 5, 6}, {4, 5, 6}});
        Matrix B({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}});
        Matrix C(3, 3);

        C.print();

        std::cout << "Macierz A:\n";
        A.print();

        std::cout << "\nMacierz B:\n";
        B.print();

        std::cout << "\nA + B:\n";
        (A + C).print();

        std::cout << "\nA - B:\n";
        (A - B).print();

        std::cout << "\nA * B:\n";
        (A * B).print();

        std::cout << "\nWyznacznik macierzy A: " << A.wyznacznik() << "\n";
    }
    // Obsluga bledu
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}