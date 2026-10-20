#pragma once
#include <iostream>
#include<vector>
#include <cstdint>
#include<fstream>

class Currency {
public:


    enum class Code : uint32_t {
        USD = 1,   // US Dollar
        AFN = 2,   // Afghanistan Afghani
        ALL = 3,   // Albania Lek(e)
        DZD = 4,   // Algerian Dinar
        EUR = 5,   // Euro
        AOA = 6,   // Angolan Kwanza
        XCD = 7,   // E.C. Dollar
        ARS = 8,   // Argentine Peso
        AMD = 9,   // Armenian Dram
        AWG = 10,  // Aruban Guilder
        AUD = 11,  // Australian Dollar
        AZN = 12,  // Azerbaijan Manat
        BSD = 13,  // Bahamian Dollar
        BHD = 14,  // Bahraini Dinar
        BDT = 15,  // Bangladesh Taka
        BBD = 16,  // Barbados Dollar
        BYN = 17,  // Belarusian Ruble
        BZD = 18,  // Belize Dollar
        XOF = 19,  // CFA Franc (West)
        BMD = 20,  // Bermuda Dollar
        BTN = 21,  // Bhutan Ngultrum
        BOB = 22,  // Bolivia Boliviano
        BAM = 23,  // Bosnia and Herzegovina Convertible Mark
        BWP = 24,  // Botswana Pula
        BRL = 25,  // Brazilian Real
        BND = 26,  // Brunei Dollar
        BGN = 27,  // Bulgarian Lev
        BIF = 28,  // Burundi Franc
        KHR = 29,  // Cambodian Riel
        XAF = 30,  // CFA Franc (Central)
        CAD = 31,  // Canadian Dollar
        CVE = 32,  // Cape Verde Escudo
        KYD = 33,  // Cayman Is. Dollar
        CLP = 34,  // Chilean Peso
        CNY = 35,  // Chinese Renminbi
        COP = 36,  // Colombian Peso
        KMF = 37,  // Comoros Franc
        CDF = 38,  // Congo Franc, Dem. Rep.of
        NZD = 39,  // New Zealand Dollar
        CRC = 40,  // Costa Rica Colon
        HRK = 41,  // Croatia Kuna
        CUP = 42,  // Cuban Peso
        CZK = 43,  // Czech Koruna
        DKK = 44,  // Danish Krone
        DJF = 45,  // Djibouti Francs
        DOP = 46,  // Dominican Peso
        EGP = 47,  // Egyptian Pound
        ERN = 48,  // Eritrea Nakfa
        SZL = 49,  // Lilangeni
        ETB = 50,  // Ethiopian Birr
        FJD = 51,  // Fiji Dollar
        XPF = 52,  // CFP Franc
        GMD = 53,  // Gambian Dalasi
        GEL = 54,  // Georgian Lari
        GHS = 55,  // Ghana Cedi
        GIP = 56,  // Gibraltar Pound
        GTQ = 57,  // Guatemala Quetzal(es)
        GNF = 58,  // Guinean Franc
        GYD = 59,  // Guyana Dollar
        HTG = 60,  // Haiti Gourde
        HNL = 61,  // Honduras Lempira
        HKD = 62,  // HongKong Dollar
        HUF = 63,  // Hungary Forint
        ISK = 64,  // Iceland Krona
        INR = 65,  // Indian Rupee
        IDR = 66,  // Indonesia Rupiah
        IRR = 67,  // Iranian Rial
        IQD = 68,  // Iraqi Dinar
        ILS = 69,  // Israel Shekel
        JMD = 70,  // Jamaican Dollar
        JPY = 71,  // Japanese Yen
        JOD = 72,  // Jordanian Dinar
        KZT = 73,  // Kazakhstan Tenge
        KES = 74,  // Kenyan Shilling
        KPW = 75,  // Korean Won, North Korea
        KRW = 76,  // Korean Won, South Korea
        KWD = 77,  // Kuwaiti Dinar
        KGS = 78,  // Kyrgyzstan Som
        LAK = 79,  // Laos Kip
        LBP = 80,  // Lebanese Pound
        LSL = 81,  // Lesotho Loti
        LRD = 82,  // Liberian Dollar
        LYD = 83,  // Libyan Dinar
        CHF = 84,  // Swiss Franc
        MOP = 85,  // Macao Pataca
        MGA = 86,  // Malagasy Ariary, Madagascar
        MWK = 87,  // Malawi Kwacha
        MYR = 88,  // Malaysia Ringgit
        MVR = 89,  // Maldives Rufiyaa
        MRU = 90,  // Mauritania Ouguiya
        MUR = 91,  // Mauritius Rupee
        MXN = 92,  // Mexican Peso
        MDL = 93,  // Moldovan Leu
        MNT = 94,  // Mongolia Tugrik
        MAD = 95,  // Morocco Dirham
        MZN = 96,  // Mozambique Metical
        MMK = 97,  // Myanmar Kyat
        NAD = 98,  // Namibia Dollar
        NPR = 99,  // Nepalese Rupee
        ANG = 100, // Netherlands Antilles Guilder
        NIO = 101, // Nicaragua Cordoba Oro
        NGN = 102, // Nigeria Naira
        MKD = 103, // Denar
        NOK = 104, // Norwegian Krone
        OMR = 105, // Oman Rial
        PKR = 106, // Pakistani Rupee
        PAB = 107, // Panama Balboa
        PGK = 108, // Kina
        PYG = 109, // Paraguay Guarani
        PEN = 110, // Sol
        PHP = 111, // Philippine Peso
        PLN = 112, // Poland Zloty
        QAR = 113, // Qatari Rial
        RON = 114, // Romanian Leu
        RUB = 115, // Russian Rouble
        RWF = 116, // Rwanda Franc
        SHP = 117, // St.Helena Pound
        WST = 118, // Samoa Tala
        STN = 119, // Sao Tome Principe Dobra
        SAR = 120, // Saudi Riyal
        RSD = 121, // Serbian Dinar
        SCR = 122, // Seychelles Rupee
        SLE = 123, // Sierra Leone Leone
        SGD = 124, // Singapore Dollar
        SBD = 125, // Solomon Is. Dollar
        SOS = 126, // Somali Shilling
        ZAR = 127, // South Africa Rand
        SSP = 128, // South Sudanese Pound
        LKR = 129, // Sri Lanka Rupee
        SDG = 130, // Sudanese Pound
        SRD = 131, // Surinamese Dollar
        SEK = 132, // Swedish Krona
        SYP = 133, // Syrian Pound
        TJS = 134, // Tajikistan Somoni
        TZS = 135, // Tanzania Shilling
        THB = 136, // Thai Baht
        TOP = 137, // Tonga Pa'anga
        TTD = 138, // Trinidad and Tobago Dollar
        TND = 139, // Tunisian Dinar
        TRY = 140, // Turkish Lira
        TMT = 141, // Turkmenistan Manat
        UGX = 142, // Uganda Shilling
        UAH = 143, // Ukraine Hryvnia
        AED = 144, // United Arab Emirates Dirham
        GBP = 145, // U.K. Pound
        UYU = 146, // Uruguay Peso
        UZS = 147, // Uzbekistan Sum
        VUV = 148, // Vanuatu Vatu
        VES = 149, // Bolivar Digital
        VND = 150, // Viet Nam Dong
        YER = 151, // Yemeni Rial
        ZMW = 152, // Zambia Kwacha
        ZWL = 153, // Zimbabwe RTGS dollar
    };
    enum class ObjectMode : uint8_t
    {
        EmptyMode = 1, ExistingMode = 2
    };

private:
    std::string m_Country;
    Code m_Code;
    std::string m_Name;
    double m_Rate;
    ObjectMode m_mode;

    std::string _CodeToString(Code code) {

        std::string CurrencyNames[] = { "USD", "AFN", "ALL", "DZD", "EUR", "AOA", "XCD", "ARS", "AMD","AWG", "AUD", "AZN", "BSD", "BHD", "BDT", "BBD", "BYN", "BZD", "XOF",
 "BMD", "BTN", "BOB", "BAM", "BWP", "BRL", "BND", "BGN", "BIF", "KHR",
 "XAF", "CAD", "CVE", "KYD", "CLP", "CNY", "COP", "KMF", "CDF", "NZD",
 "CRC", "HRK", "CUP", "CZK", "DKK", "DJF", "DOP", "EGP", "ERN", "SZL",
 "ETB", "FJD", "XPF", "GMD", "GEL", "GHS", "GIP", "GTQ", "GNF", "GYD",
 "HTG", "HNL", "HKD", "HUF", "ISK", "INR", "IDR", "IRR", "IQD", "ILS",
 "JMD", "JPY", "JOD", "KZT", "KES", "KPW", "KRW", "KWD", "KGS", "LAK",
 "LBP", "LSL", "LRD", "LYD", "CHF", "MOP", "MGA", "MWK", "MYR", "MVR",
 "MRU", "MUR", "MXN", "MDL", "MNT", "MAD", "MZN", "MMK", "NAD", "NPR",
 "ANG", "NIO", "NGN", "MKD", "NOK", "OMR", "PKR", "PAB", "PGK", "PYG",
 "PEN", "PHP", "PLN", "QAR", "RON", "RUB", "RWF", "SHP", "WST", "STN",
 "SAR", "RSD", "SCR", "SLE", "SGD", "SBD", "SOS", "ZAR", "SSP", "LKR",
 "SDG", "SRD", "SEK", "SYP", "TJS", "TZS", "THB", "TOP", "TTD", "TND",
 "TRY", "TMT", "UGX", "UAH", "AED", "GBP", "UYU", "UZS", "VUV", "VES",
 "VND", "YER", "ZMW", "ZWL"
        };

        return CurrencyNames[(static_cast<size_t>(code) - 1)];
    }
    std::string _CodeToCurrencyName(Code code) {

        std::string CurrencyNames[] = {
            "US Dollar", "Afghanistan Afghani", "Albania Lek(e)", "Algerian Dinar", "Euro",
            "Angolan Kwanza", "E.C. Dollar", "Argentine Peso", "Armenian Dram", "Aruban Guilder",
            "Australian Dollar", "Azerbaijan Manat", "Bahamian Dollar", "Bahraini Dinar", "Bangladesh Taka",
            "Barbados Dollar", "Belarusian Ruble", "Belize Dollar", "CFA Franc", "Bermuda Dollar",
            "Bhutan Ngultrum", "Bolivia Boliviano", "Bosnia and Herzegovina Convertible Mark", "Botswana Pula", "Brazilian Real",
            "Brunei Dollar", "Bulgarian Lev", "Burundi Franc", "Cambodian Riel", "CFA Franc",
            "Canadian Dollar", "Cape Verde Escudo", "Cayman Is. Dollar", "Chilean Peso", "Chinese Renminbi",
            "Colombian Peso", "Comoros Franc", "Congo Franc, Dem. Rep.of", "New Zealand Dollar", "Costa Rica Colon",
            "Croatia Kuna", "Cuban Peso", "Czech Koruna", "Danish Krone", "Djibouti Francs",
            "Dominican Peso", "Egyptian Pound", "Eritrea Nakfa", "Lilangeni", "Ethiopian Birr",
            "Fiji Dollar", "CFP Franc", "Gambian Dalasi", "Georgian Lari", "Ghana Cedi",
            "Gibraltar Pound", "Guatemala Quetzal(es)", "Guinean Franc", "Guyana Dollar", "Haiti Gourde",
            "Honduras Lempira", "HongKong Dollar", "Hungary Forint", "Iceland Krona", "Indian Rupee",
            "Indonesia Rupiah", "Iranian Rial", "Iraqi Dinar", "Israel Shekel", "Jamaican Dollar",
            "Japanese Yen", "Jordanian Dinar", "Kazakhstan Tenge", "Kenyan Shilling", "Korean Won, North Korea",
            "Korean Won, South Korea", "Kuwaiti Dinar", "Kyrgyzstan Som", "Laos Kip", "Lebanese Pound",
            "Lesotho Loti", "Liberian Dollar", "Libyan Dinar", "Swiss Franc", "Macao Pataca",
            "Malagasy Ariary, Madagascar", "Malawi Kwacha", "Malaysia Ringgit", "Maldives Rufiyaa", "Mauritania Ouguiya",
            "Mauritius Rupee", "Mexican Peso", "Moldovan Leu", "Mongolia Tugrik", "Morocco Dirham",
            "Mozambique Metical", "Myanmar Kyat", "Namibia Dollar", "Nepalese Rupee", "Netherlands Antilles Guilder",
            "Nicaragua Cordoba Oro", "Nigeria Naira", "Denar", "Norwegian Krone", "Oman Rial",
            "Pakistani Rupee", "Panama Balboa", "Kina", "Paraguay Guarani", "Sol",
            "Philippine Peso", "Poland Zloty", "Qatari Rial", "Romanian Leu", "Russian Rouble",
            "Rwanda Franc", "St.Helena Pound", "Samoa Tala", "Sao Tome Principe Dobra", "Saudi Riyal",
            "Serbian Dinar", "Seychelles Rupee", "Sierra Leone Leone", "Singapore Dollar", "Solomon Is. Dollar",
            "Somali Shilling", "South Africa Rand", "South Sudanese Pound", "Sri Lanka Rupee", "Sudanese Pound",
            "Surinamese Dollar", "Swedish Krona", "Syrian Pound", "Tajikistan Somoni", "Tanzania Shilling",
            "Thai Baht", "Tonga Pa'anga", "Trinidad and Tobago Dollar", "Tunisian Dinar", "Turkish Lira",
            "Turkmenistan Manat", "Uganda Shilling", "Ukraine Hryvnia", "United Arab Emirates Dirham", "U.K. Pound",
            "Uruguay Peso", "Uzbekistan Sum", "Vanuatu Vatu", "Bolivar Digital", "Viet Nam Dong",
            "Yemeni Rial", "Zambia Kwacha", "Zimbabwe RTGS dollar"
        };

        return CurrencyNames[(static_cast<size_t>(code) - 1)];
    }
   

    //using FileHandler
    void _SaveExistingObject();
    void _SaveNewObject();


public:

    Currency(std::string country, uint32_t code, std::string name, double rate, ObjectMode mode) : m_Country(std::move(country)), m_Code(static_cast<Code>(code)), m_Name(std::move(name)), m_Rate(rate), m_mode(mode) {};


   
    const std::string &GetCountry() const noexcept {
        return m_Country;
    }

    std::string GetCode() {
        return _CodeToString(m_Code);
    }

    bool isEmpty() const noexcept {
        return (m_mode == ObjectMode::EmptyMode);
    }

    std::string GetName() {
        return _CodeToCurrencyName(m_Code);
    }

    double GetRate() noexcept {
        return m_Rate;
    }

    void SetRate(double newRate) {
        m_Rate = newRate;
    }

    void Save() {

        switch (m_mode)
        {
        case ObjectMode::EmptyMode:
        {
            return;
        }
        case ObjectMode::ExistingMode:
        {
            _SaveExistingObject();
            break;
        }

        default:
            return;

        }


    };

};