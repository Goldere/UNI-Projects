
class Laiva:

    def __init__(self, koordinaatti_1, koordinaatti_2="Ei koordinaattia", koordinaatti_3="Ei koordinaattia", koordinaatti_4="Ei koordinaattia", koordinaatti_1_osuttu=False, koordinaatti_2_osuttu=False,
                 koordinaatti_3_osuttu=False, koordinaatti_4_osuttu=False, upotettu=False,):

        self.__upotettu = upotettu
        self.__koordinaatti_1 = koordinaatti_1
        self.__koordinaatti_2 = koordinaatti_2
        self.__koordinaatti_3 = koordinaatti_3
        self.__koordinaatti_4 = koordinaatti_4
        self.__koordinaatti_1_osuttu = koordinaatti_1_osuttu
        self.__koordinaatti_2_osuttu = koordinaatti_2_osuttu
        self.__koordinaatti_3_osuttu = koordinaatti_3_osuttu
        self.__koordinaatti_4_osuttu = koordinaatti_4_osuttu

    def osuma_laivaan(self, koordinaatti):
        """
        :param koordinaatti:
        :return: Palauttaa true, jos ammus osui laivaan.
        """
        if self.__koordinaatti_1 == koordinaatti.upper():
           self.__koordinaatti_1_osuttu = True
           return True
        elif self.__koordinaatti_2 == koordinaatti.upper():
            self.__koordinaatti_2_osuttu = True
            return True
        elif self.__koordinaatti_3 == koordinaatti.upper():
            self.__koordinaatti_3_osuttu = True
            return True
        elif self.__koordinaatti_4 == koordinaatti.upper():
            self.__koordinaatti_4_osuttu = True
            return True
        else:
            return False

    def laiva_upotettu(self):
        """
        :return: Palauttaan true, jos laiva on upotettu.
        """
        if self.__upotettu == True:
            return True
    def tarkista_upotus(self, koordinaatti):

        # Submarine
        if self.__koordinaatti_2 == "Ei koordinaattia" and self.__koordinaatti_3 == "Ei koordinaattia" and self.__koordinaatti_4 == "Ei koordinaattia":
            if koordinaatti == self.__koordinaatti_1:
                if self.__koordinaatti_1_osuttu == True:
                    self.__upotettu = True
                    return True
        # Destroyer
        if self.__koordinaatti_3 == "Ei koordinaattia" and self.__koordinaatti_4 == "Ei koordinaattia":
            if koordinaatti == self.__koordinaatti_1 or koordinaatti == self.__koordinaatti_2:
                if self.__koordinaatti_1_osuttu == True and self.__koordinaatti_2_osuttu == True:
                    self.__upotettu = True
                    return True

        # Cruiser
        if self.__koordinaatti_4 == "Ei koordinaattia":
            if koordinaatti == self.__koordinaatti_1 or koordinaatti == self.__koordinaatti_2 or koordinaatti == self.__koordinaatti_3:
                if self.__koordinaatti_1_osuttu == True and self.__koordinaatti_2_osuttu == True and self.__koordinaatti_3_osuttu == True:
                    self.__upotettu = True
                    return True

        # Battleship
        if koordinaatti == self.__koordinaatti_1 or koordinaatti == self.__koordinaatti_2 or koordinaatti == self.__koordinaatti_3 or koordinaatti == self.__koordinaatti_4:
            if self.__koordinaatti_1_osuttu == True and self.__koordinaatti_2_osuttu == True and self.__koordinaatti_3_osuttu == True and self.__koordinaatti_4_osuttu == True:
                self.__upotettu = True
                return True

    def __str__(self):
        return f"{self.__koordinaatti_1, self.__koordinaatti_2, self.__koordinaatti_3, self.__koordinaatti_4}"

def laivat_upotettu(sanakirja):
    """
    :param sanakirja:
    :return: Palauttaa true, jos KAIKKI laivat ovat upotettu, muulloin false.
    """
    laiva_upottamatta = False
    for laiva, koordinaatit in sanakirja.items():
        if not sanakirja[laiva].laiva_upotettu():
            laiva_upottamatta = True

    if laiva_upottamatta == False:
        return True

def tulosta_pelilauta(pelikenttä_ruudukko):
    """
    Tulostaa pelilaudan nykyisen tilanteen.
    :param pelikenttä_ruudukko:
    :return:
    """
    print("")
    print(f"  A B C D E F G H I J")
    for koordinaatti, arvo in pelikenttä_ruudukko.items():
        if koordinaatti[0] == "J":
            print(f"{arvo} {koordinaatti[1]}")
        elif koordinaatti[0] == "A":
            print(f"{koordinaatti[1]} {arvo}", end=' ')
        else:
            print(f"{arvo}", end=' ')
    print(f"  A B C D E F G H I J")
    print("")

def duplikaatteja(lista):
    """
    :param lista:
    :return: Palauttaa false, jos annetussa listassa on arvoja, jotka ovat identtisiä toistensa kansswa.
    """
    nähty = set()
    for koordinaatti in lista:
        if koordinaatti in nähty:
            return True
        nähty.add(koordinaatti)
    return False

def koordinaatti_virheellinen_tai_laivaa_ammutaan(ammuttu_koordinaatti, pelikenttä_ruudukko, peli_kirjaimet, laivat_sanakirja):
    """
    :param ammuttu_koordinaatti:
    :param pelikenttä_ruudukko:
    :param peli_kirjaimet:
    :param laivat_sanakirja:
    :return: Jos koordinaatti on virheellinen, palauteteen True.
    Jos koordinaatissa on laiva, jota on JO ammuttu, palautetaan True.
    """
    if len(ammuttu_koordinaatti) == 2 and ammuttu_koordinaatti[1].isdigit() and str(ammuttu_koordinaatti[0].upper()) in peli_kirjaimet:
        ammutaan_laivaa = False
        for key in laivat_sanakirja:
            if str(key[0].upper()) == pelikenttä_ruudukko[ammuttu_koordinaatti.upper()]:
                ammutaan_laivaa = True
        if pelikenttä_ruudukko[ammuttu_koordinaatti.upper()] == "*" or ammutaan_laivaa == True or pelikenttä_ruudukko[
            ammuttu_koordinaatti.upper()] == "X":
            print("Location has already been shot at!")
            return True
        elif str(ammuttu_koordinaatti[0].upper()) in peli_kirjaimet and 0 <= int(ammuttu_koordinaatti[1]) <= 9:
            return False
    else:
        print("Invalid command!")
        return True

def main():

    """
    Pää funktio laivanupotukseen.
    :return:
    """

    filename = input("Enter file name: ")

    peli_kirjaimet = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J"]

    # Luodaan lista laivojen koordinaateista, jotta voidaan tarkista ovat ne päällekäin
    laivojen_koordinaatit = []

    # Luodaan sanakirja, jossa avaimeksi tulee laivan nimi ja arvoiksi sen koordinaatit
    laivat_sanakirja = {}

    try:

        file = open(filename, mode="r")

        for line in file:
            line = line.rstrip()
            osat = line.split(";")

            # Tarkistetaan ovatko laivan koordinaatit virheelliset
            for laivan_koordinaatti in osat:
                if len(laivan_koordinaatti) == 2:
                    if not str(laivan_koordinaatti[0].upper()) in peli_kirjaimet or not 0 <= int(laivan_koordinaatti[1]) <= 9:
                        print("Error in ship coordinates!")
                        return
                if len(laivan_koordinaatti) == 3:
                    print("Error in ship coordinates!")
                    return

            def lisaa_arvot_laivojen_koordinaatteihin(koordinaatti1, koordinaatti2, koordinaatti3, koordinaatti4):
                """
                Lisää koordinaatit laivojen_koordinaatit listaan.
                :param osa1:
                :param osa2:
                :param osa3:
                :param osa4:
                :return:
                """
                if koordinaatti1:
                    laivojen_koordinaatit.append(koordinaatti1)
                if koordinaatti2:
                    laivojen_koordinaatit.append(koordinaatti2)
                if koordinaatti3:
                    laivojen_koordinaatit.append(koordinaatti3)
                if koordinaatti4:
                    laivojen_koordinaatit.append(koordinaatti4)

            if len(osat) == 4:
                uusi_laiva = Laiva(osat[1].upper(), osat[2].upper(), osat[3].upper())
                lisaa_arvot_laivojen_koordinaatteihin(osat[1], osat[2], osat[3], None)
            elif len(osat) == 3:
                uusi_laiva = Laiva(osat[1].upper(), osat[2].upper())
                lisaa_arvot_laivojen_koordinaatteihin(osat[1], osat[2], None, None)
            elif len(osat) == 2:
                uusi_laiva = Laiva(osat[1].upper())
                laivojen_koordinaatit.append(osat[1])
            else:
                uusi_laiva = Laiva(osat[1].upper(), osat[2].upper(), osat[3].upper(), osat[4].upper())
                lisaa_arvot_laivojen_koordinaatteihin(osat[1], osat[2], osat[3], osat[4])
            laivan_nimi = str(osat[0])
            laivan_numero = 1

            while laivan_nimi in laivat_sanakirja:
                laivan_nimi = laivan_nimi + str(laivan_numero)
                laivan_numero += 1
            laivat_sanakirja[laivan_nimi] = uusi_laiva

        # Tarkistetaan ovatko laivat päällekkäin
        if duplikaatteja(laivojen_koordinaatit):
            print("There are overlapping ships in the input file!")
            return

    except OSError:
        print("File can not be read!")

        return

    # Luodaan pelikentälle koordinaatit
    # Jokainen sanakirjan avain on yksi peli kentän koordinaatti ja arvot ovat tyhjiä merkkijonoja, joihin pelaaja ampuu.
    pelikenttä_ruudukko = {}

    for koordinaatin_numero in range(0, 10):
        for pelikentän_kirjain in peli_kirjaimet:
            pelikentän_koordinaatti = pelikentän_kirjain + str(koordinaatin_numero)
            pelikenttä_ruudukko[pelikentän_koordinaatti] = " "
        koordinaatin_numero += 1

    # Tulostetaan pelilauta
    tulosta_pelilauta(pelikenttä_ruudukko)

    while True:

        ammuttu_koordinaatti = input("Enter place to shoot (q to quit): ")

        if ammuttu_koordinaatti == "q" or ammuttu_koordinaatti == "Q":
            print("Aborting game!")
            break

        # Tarkistetaan onko käyttäjän antama koordinaatti virheellinen tai ammutaanko laivaa, jota on aikasemmin ammuttu.
        if not koordinaatti_virheellinen_tai_laivaa_ammutaan(ammuttu_koordinaatti, pelikenttä_ruudukko, peli_kirjaimet, laivat_sanakirja):

            # Tarkistetaan onko ammuksen kohde laiva.
                for laiva, koordinaatit in laivat_sanakirja.items():
                    if laivat_sanakirja[laiva].osuma_laivaan(ammuttu_koordinaatti):
                        # Merkitään osuma
                        pelikenttä_ruudukko[ammuttu_koordinaatti.upper()] = "X"

                        # Tarkistetaan onko laiva upotettu
                        if laivat_sanakirja[laiva].tarkista_upotus(ammuttu_koordinaatti.upper()):
                            laiva_ilman_numeroa = ""
                            laiva_tekstinä = str(laiva)
                            for i in laiva_tekstinä:
                                if not i.isdigit():
                                    laiva_ilman_numeroa = laiva_ilman_numeroa + i
                            print(f"You sank a {laiva_ilman_numeroa}!")
                        break
                    else:
                        pelikenttä_ruudukko[ammuttu_koordinaatti.upper()] = "*"

        # Muokkaa pelikentän arvot, jos laiva on upotettu.
        for koordinaatti, _ in pelikenttä_ruudukko.items():
            for laiva, koordinaatit in laivat_sanakirja.items():
                if laivat_sanakirja[laiva].tarkista_upotus(koordinaatti):
                    pelikenttä_ruudukko[koordinaatti] = str(laiva[0]).upper()

        # Tulostetaan pelilauta
        tulosta_pelilauta(pelikenttä_ruudukko)

        # Tarkistaa onko kaikki laivat upotettu
        if laivat_upotettu(laivat_sanakirja):
            print("Congratulations! You sank all enemy ships.")
            break

if __name__ == "__main__":
    main()