# Client HTTP în C pentru Interacțiunea cu un API REST

Acest proiect implementeza un client HTTP în C pentru interacțiunea cu un API REST. Serverul, reprezentând o infrastructură existentă, expune un API REST simulând o bibliotecă online de cărți. Programul client permite utilizatorilor să introducă comenzi prin interfața cu linia de comandă (CLI) pentru a interacționa cu biblioteca virtuală.

## Cuprins

- [Descriere](#descriere)
- [Caracteristici](#caracteristici)
- [JsonParser](#JsonParser)

## Descriere

Descriere bazata pe structura codului;
                                                                                                                                                    
┌──(kali㉿kali)-[~/Pcom/homework4-public]
└─$ tree .

├── homework4-public
│   ├── buffer.c
│   ├── buffer.h
│   ├── checker
│   │   └── checker.py
│   ├── client
│   ├── client.c
│   ├── helpers.c
│   ├── helpers.h
│   ├── Makefile
│   ├── Readme.md
│   ├── README.md
│   ├── requests.c
│   └── requests.h



client.c = Crearea Socket-ului: Programul client începe prin crearea unui socket, care este un punct final pentru comunicare.
Conectarea la Server: Clientul se conectează apoi la server folosind funcția connect(), specificând adresa și numărul de port ale serverului.
Comunicare de Date: Odată ce conexiunea este stabilită, clientul poate trimite date către server folosind funcția send() și poate primi date de la server folosind funcția recv().Închiderea Socket-ului: După ce comunicarea este finalizată, clientul închide socket-ul.Functiile mentionate sunt modificate pe cerinta in alte fisiere. Comunicarea este realizata dupa urmatoarea logica: utilizatorul da o comanda serverului. Comenzile acceptate sunt specificate. // Comanda acceptata este register
   printf("Comanda acceptata este:\n"
       "register - efectuează înregistrarea\n"
       "login - efectuează autentificarea\n"
       "enter_library - cere acces în bibliotecă\n"
       "get_books - cere toate cărţile de pe server\n"
       "get_book - cere informaţie despre o carte\n"
       "add_book - adaugă o carte\n"
       "delete_book - șterge o carte\n"
       "logout - efectueaza delogarea utilizatorului\n"
       "exit - efectueaza ieșirea din program\n"); 
Dupa aceea va deschide conexiune cu serverul in functie de comanda data. Se respecta conditiile pentru logare si register. Am considerat user invalid ca fiind orice username cu caractere ciudate: ",.\":;/][\\{})!@#$^&*()_'+=<>?|]"). Clientul va trimite mesaje de eroare care contin codeul ERROR pentru orice imput invalid. Pentru comanda de add_book am considerat ca imput invalid orice este newline si atat. Pentru imput invali la add_book se va afisa un mesaj care contine cuvantul ERROR.Alta situatie abordata la add book command este cand ID-ul este invalid. Un id invalid poate fi negativ sau 0. Se va afisa un mesaj de eroare care contine cuvantul ERROR. Mesaje de eroare se vor afisa continand textul ERROR si pentru urmatoarele responsuri primite de la server:  // error string codes
   char *error_string = "400 Bad Request\n"
                     "401 Unauthorized\n"
                     "402 Payment Required\n"
                     "403 Forbidden\n"
                     "404 Not Found\n"
                     "405 Method Not Allowed\n"
                     "406 Not Acceptable\n"
                     "407 Proxy Authentication Required\n"
                     "408 Request Timeout\n"
                     "409 Conflict\n"
                     "410 Gone\n"
                     "411 Length Required\n"
                     "412 Precondition Failed\n"
                     "413 Payload Too Large\n"
                     "414 URI Too Long\n"
                     "415 Unsupported Media Type\n"
                     "416 Range Not Satisfiable\n"
                     "417 Expectation Failed\n"
                     "418 I'm a Teapot\n"
                     "421 Misdirected Request\n"
                     "422 Unprocessable Entity\n"
                     "423 Locked\n"
                     "424 Failed Dependency\n"
                     "425 Too Early\n"
                     "426 Upgrade Required\n"
                     "428 Precondition Required\n"
                     "429 Too Many Requests\n"
                     "431 Request Header Fields Too Large\n"
                     "451 Unavailable For Legal Reasons\n";
Alte erori o sa fie afisate atunci cand nu s-a obtinut tokenul / cookie ul de sesiune sau cand utilizatorul nu este logat.
Atunci cand a comanda a fost efectuata cu SUCESS. Se va afisa un mesaj care contine cuvantul SUCCESS. Exista comenzi care afiseaza si anumite informatii cum ar fi nume de carti si detalii despre ele.

requests.c = Acest fișier conține implementările funcțiilor necesare pentru generarea cererilor HTTP de tip GET și POST în cadrul unei aplicații de client HTTP. Iată o descriere a fiecărei funcții:
Funcția compute_get_request:
Această funcție generează un mesaj HTTP de tip GET pentru a fi trimis către server.
Parametrii acestei funcții includ adresa IP a serverului (host), calea către resursa dorită (url), parametrii opționali ai cererii (query_params), cookie-urile pentru autentificare (cookies), numărul de cookie-uri (cookies_count) și token-ul de autentificare (token).
Mesajul HTTP generat include metoda GET, URL-ul resursei, antetul Host, antetul Connection și, dacă este necesar, antetul Cookie pentru autentificare și antetul Authorization pentru autorizare.
În funcție de parametrii specificați, mesajul poate include diferite antete și cookie-uri. Functia poate gestion si cereri de tip DELETE.
Funcția compute_post_request:
Această funcție generează un mesaj HTTP de tip POST pentru a fi trimis către server.
Parametrii acestei funcții includ adresa IP a serverului (host), calea către resursa dorită (url), tipul de conținut al cererii (content_type), datele de corp ale cererii (body_data), numărul de câmpuri de date de corp (body_data_fields_count), cookie-urile pentru autentificare (cookies), numărul de cookie-uri (cookies_count), cheile pentru datele de corp (keys) și token-ul de autentificare (token).
Mesajul HTTP generat include metoda POST, URL-ul resursei, antetul Host, antetul Content-Type, antetul Connection și, dacă este necesar, antetul Authorization pentru autorizare.
De asemenea, mesajul include datele de corp ale cererii sub forma unui obiect JSON, formatat conform specificațiilor HTTP.
Funcția poate trata diferite tipuri de cereri POST, în funcție de parametrii specificați.

helpers.c = Acest fișier conține o serie de funcții utile pentru gestionarea comunicării client-server în cadrul unei aplicații HTTP. Iată o descriere a fiecărei funcții:
Funcția error:
Această funcție este utilizată pentru afișarea mesajelor de eroare.
Prin intermediul funcției perror, mesajul specificat ca parametru este afișat împreună cu un mesaj de eroare specificat de sistemul de operare.
Apoi, programul este întrerupt utilizând funcția exit.
Funcția compute_message:
Această funcție concatenează un șir de caractere (line) la un mesaj existent (message) și adaugă un terminator de linie.
Funcția open_connection:
Această funcție creează o conexiune către un server utilizând adresa IP (host_ip) și portul (portno) specificate.
Este creat un socket utilizând funcția socket, iar apoi este inițializată structura serv_addr cu informațiile despre server.
Conexiunea este realizată utilizând funcția connect.
Funcția close_connection:
Această funcție închide conexiunea la socket specificată prin parametrul sockfd, utilizând funcția close.
Funcția send_to_server:
Această funcție trimite un mesaj (message) către server, utilizând socketul specificat prin parametrul sockfd.
Mesajul este trimis în blocuri până când întregul mesaj este trimis cu succes.
Funcția receive_from_server:
Această funcție primește un mesaj de la server utilizând socketul specificat prin parametrul sockfd.
Mesajul este citit din socket și adăugat într-un buffer până când este primit întregul mesaj.
Se verifică dacă mesajul conține antetul și conținutul, iar apoi se extrage conținutul efectiv al mesajului.

*urmatoarele functii sunt un parser de json*
Funcția basic_extract_json_response:
Această funcție returnează un pointer către prima apariție a unui obiect JSON într-un șir de caractere (str).
Funcția create_json_data:
Această funcție construiește un obiect JSON utilizând cheile și valorile specificate în parametri (keys și values) și numărul total de intrări (num_entries).

*urmatoarele functii analizeaza si extrag fragmente din response-ul de la server*
Funcția get_cookies:
Această funcție extrage cookie-urile dintr-un mesaj HTTP de răspuns și le stochează într-un șir de caractere (cookies).
Funcția get_token:
Această funcție extrage token-ul de autentificare dintr-un șir JSON (json_str) și îl stochează într-un șir de caractere (token).
Funcția is_error_response:
Această funcție verifică dacă un mesaj HTTP de răspuns indică o eroare, comparându-l cu un șir de caractere care conține codurile de eroare HTTP.

buffer.c = Acest fișier conține o implementare a unei structuri de tip buffer și a unor funcții utile pentru lucrul cu acest buffer. Iată o descriere pentru fiecare funcție:
Funcția buffer_init:
Această funcție inițializează un obiect de tip buffer, setând câmpurile data și size la valorile implicite (NULL și 0, respectiv).
Returnează buffer-ul inițializat.
Funcția buffer_destroy:
Această funcție eliberează memoria alocată pentru datele din buffer (buffer->data) și setează mărimea buffer-ului la 0.
Este importantă pentru eliberarea resurselor atunci când un buffer nu mai este necesar.
Funcția buffer_is_empty:
Această funcție returnează o valoare booleană care indică dacă buffer-ul este gol sau nu (adică, dacă câmpul data este NULL).
Funcția buffer_add:
Această funcție adaugă date în buffer.
Dacă buffer-ul are deja date, memoria este realocată pentru a-i face loc datelor noi; altfel, este alocată inițial.
Datele noi sunt copiate în buffer la sfârșitul acestuia, iar mărimea buffer-ului este actualizată corespunzător.
Funcția buffer_find:
Această funcție caută o secvență specificată de date (data) în buffer și returnează poziția primei apariții a acestei secvențe.
Căutarea este făcută cu o sensibilitate la litere mari/mici.
Funcția buffer_find_insensitive:
Această funcție este similară cu buffer_find, dar face căutarea insensibilă la litere mari/mici, adică nu face diferență între literele mari și mici.

## Caracteristici

- Interacționează cu un server API REST folosind cereri HTTP.
- Acceptă comenzi de la utilizator din interfața cu linia de comandă (CLI).
- Oferă o experiență simulată de bibliotecă online, permițând utilizatorilor să efectueze acțiuni precum căutarea de cărți, adăugarea de cărți în bibliotecă și gestionarea sesiunilor.

## JsonParser

Pentru partea din cerinta: "Astfel, pentru a parsa răspunsurile primite de la server, puteţi (şi e recomandat) să folosiţi o bibliotecă. Vă sugerăm parson pentru C sau nlohmann pentru C++, însă puteţi folosi orice (inclusiv o soluţie proprie), justificând alegerea în README."

Am creat o implementare minimalista a unui parser de json.
*MOTIVATIE*:
Alegerea de a crea propriul parser JSON în loc să folosesc biblioteci precum Parson pentru C sau nlohmann pentru C++ poate fi justificată din mai multe motive:
1. Cunoașterea profundă a procesului de parsare: Creând propriul parser, am dobândi o înțelegere mai detaliată a modului în care funcționează procesul de parsare JSON. Acest lucru m-ar ajuta să înțeleg mai bine subtilitățile acestui proces și să pot face optimizări specifice nevoilor proiectului meu.
2. Control absolut asupra funcționalității: Prin dezvoltarea propriului parser, aș avea control total asupra funcționalității acestuia. Acest lucru m-ar permite să adaptez parser-ul pentru cerințele specifice ale proiectului meu și să îl extind pentru a gestiona situații sau formate JSON mai puțin comune.
3. Mărime redusă a bibliotecii: Unele proiecte pot avea restricții de spațiu. Dezvoltând propriul parser, aș putea crea o soluție mai ușoară și mai compactă decât utilizând o bibliotecă externă care poate conține funcționalități neutilizate.
4. Flexibilitate : Aș avea libertatea de a schimba și ajusta codul după cum doresc, fără a depinde de actualizările sau modificările unei biblioteci externe.
Functiile pe care le-am folosit eu au urmatoarele avantaje:
- implementare simpla si usor de inteles
- control asupra proiectului
- lipsa dependentelor externe
- personalizare si optimizare


