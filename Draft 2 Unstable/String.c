/*
    Implements a String Structure As Well As Functions For String Manipulation
*/

struct String{
    int length;
    char* str;
};

struct Burn_Node{
    struct Burn_Node* next;
    struct String* value;
};

typedef struct String* String;

struct Burn_Node* head = (struct Burn_Node*) NULL;
struct Burn_Node* tail = (struct Burn_Node*) NULL;

String string_like(char* from);
String string_from(char* from);
String force_string(char* from, int length);
String string_from_burn(char* from);
String force_string_burn(char* from, int length);
void add_to_burn(String s);
String append(String start, String end);
String reverse(String to_rev);
String substring(String main, int start, int stop);
String replace(String main, String replace, String with);
String combine(int amt,...);
int find(String searching, String finding);
int count(String main, String counting);
int compare(String a, String b);
int equals(String a, String b);
int hash(String to_hash);
char index_without_check(String str, int index);
char at_index(String str, int index);
void Print(String a);
void Println(String a);
void Dln(String a);
void delete(String to_del);

String replace_n(String main, String replace, String with, int n){
    /* I hate the code I wrote for this function the most */

    int replaces = count(main, replace);
    replaces = n > replaces ? replaces : n;
    int new_size = main->length - replace->length * replaces + with->length * replaces;

    String returning = malloc(sizeof(String));
    returning->str = malloc(new_size + 1);
    returning->str[new_size] = '\0';

    int main_len = main->length;
    char* main_c = main->str;

    int found = find(main, replace);
    int counter = 0;
    int fc = 0;

    while(main->length){
        if(found == fc && replaces > 0){
            replaces--;
            for(int i = 0;i < with->length;i++, counter++){
                returning->str[counter] = with->str[i];
            }

            main->str += replace->length;
            main->length -= replace->length;

            found = find(main, replace);
            fc = 0;
            continue;
        }
        
        returning->str[counter] = main->str[0];
        counter++, main->length--, main->str++;
    }

    main->length = main_len;
    main->str = main_c;

    return returning;
}

/*
Get a String structure from the char* from
Creates a copy of from
*/
String string_like(char* from){
    String returning = malloc(sizeof(String));

    int len = strlen(from);
    returning->length = len;
    len += 1;
    returning->str = malloc(len + 1); /* null terminator is not counted */

    if(returning->str == NULL){
        return NULL;
    }

    memcpy(returning->str, from, len);
    return (String) returning;
}

/*
Like string_like except no copy
*/
String string_from(char* from){
    /* only for use of strings create from malloc */
    String returning = malloc(sizeof(String));
    returning->length = strlen(from);
    returning->str = from;
    return returning;
}

/*
Like string_from except length is also given
*/
String force_string(char* from, int length){
    /* only for use of strings created from malloc */
    String returning = malloc(sizeof(String));
    returning->length = length;
    returning->str = from;
    return returning;
}

/*
    like string_like except add the string to a list of strings to be freed
*/
String string_like_burn(char* from){
    String s = string_like(from);
    add_to_burn(s);
    return s;
}

/*
    Like string_from, except add the resultant String to a list that can be destroyed by invoking burn_strings
*/
String string_from_burn(char* from){
    String s = string_from(from);
    add_to_burn(s);
    return s;
}

/*
    Like force_string except add the resultant String to a list that can be destroyed by invoking burn_strings
*/
String force_string_burn(char* from, int length){
    String s = force_string(from, length);
    add_to_burn(s);
    return s;
}

// /*
//     format to_format with ...
// */
// String format_string(String to_format, ...){
//     int args = 0;
//     int ignore_next = 0;
//     int redux_one = 0;

//     for(int i = 0;i < to_format->length;i++){
//         if(ignore_next){
//             ignore_next = 0;
//             continue;
//         }
        
//         if(to_format->str[i] == '%'){
//             if(i + 1 >= to_format->length){
//                 fprintf(stderr, "%% not escaped by %% in string \"%s\"!", to_format->str);
//                 exit(1);
//             }
//             if(to_format->str[i + 1] == '%'){
//                 ignore_next = 1;
//                 redux_one++;
//             }
//             else{
//                 args++;
//             }
//         }
//     }

//     int newlength = to_format->length - redux_one - args * 2;

//     va_list args_lit, to_pass;
//     va_start(args_lit, amt);
//     va_start(to_pass, amt);

//     for(int i = 0;i < amt;i++){
//         newlength += strlen(va_arg(args_lit, char*));
//     }

//     va_end(args_lit);

//     char* returning_str = malloc(newlength + 1);

//     snprintf(returning_str, newlength, to_format->str, to_pass);

//     return force_string(returning_str, newlength);
// }

/*
    add a string to be burned on burn_strings() invocation
*/
void add_to_burn(String s){
    return;
    if(!head){
        head = malloc(sizeof(struct Burn_Node));
        head->value = s;
        struct Burn_Node* nxt = malloc(sizeof(struct Burn_Node));
        head->next = nxt;
        tail = nxt;
        tail->next = (struct Burn_Node*) NULL;
    }
    else{
        tail->value = s;
        struct Burn_Node* nxt = malloc(sizeof(struct Burn_Node));
        tail->next = nxt;
        tail = nxt;
        tail->next = (struct Burn_Node*) NULL;
    }
}

/*
    Destroy strings in burned -- the burn is used internally
*/
void burn_strings(){
    struct Burn_Node* current_node = head;
    struct Burn_Node* temp;
    while(current_node->next){
        // printf("HERE1\n");
        // fflush(stdout);
        
        delete(current_node->value);
        
        // printf("HERE\n");
        // fflush(stdout);

        temp = current_node;
        current_node = current_node->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

/*
    combine VA arguments that are char*
*/
String combine(int amt,...){
    va_list vlist;

    va_start(vlist, amt);

    String s = string_like("");

    for(int i = 0;i < amt;i++){
        add_to_burn(s);
        s = append(s, string_like_burn(va_arg(vlist, char*)));
    }

    va_end(vlist);

    return (String) s;
}

/* 
return a new String which is end appended to start 
ie
> Str_A is
> > length = 6
> > str = "Hello \0"
> Str_B is
> > length = 5
> > str = "World\0"
> the returned value from append(Str_A, Str_B)
> > length = 11
> > str = "Hello World\0"
*/
String append(String start, String end){
    String returning = malloc(sizeof(String));
    
    int len = start->length + end->length;
    returning->length = len;
    returning->str = (char*) malloc(len + 1); /* null string is not counted in length of strings :/ */

    memcpy(returning->str, start->str, start->length);
    memcpy(returning->str + start->length, end->str, end->length + 1);
    return returning;
}

/*
    return a reversed string
    returns NULL on memory error
*/
String reverse(String to_rev){
    char* returning_str = malloc(to_rev->length + 1);
    int returning_len = to_rev->length;

    String returning = malloc(sizeof(String));

    returning->length = returning_len;
    returning->str = returning_str;
    returning->str[returning_len] = '\0';

    if(!returning || !returning_str){
        fprintf(stderr, "Out of Memory!");
        return NULL;
    }

    /* reverse - not including the null terminator */
    for(int i = 0;i < to_rev->length;i++){
        returning->str[to_rev->length - i - 1] = to_rev->str[i];
    }

    return returning;
}

/*
    return the substring formed by taking the characters of main starting at start and up to but not including stop
    ie substring(string_like("Hello"), 1, 3) -> "el" 
    raises an error if either index is greater than length + 1
*/
String substring(String main, int start, int stop){
    if(start > main->length + 1 || stop > main->length + 1 || start > stop || start < 0 || stop < 0){
        fprintf(stderr, "error on %d -- string indexes %d or %d are invalid for string of length %d", __LINE__, start, stop, main->length);
    }

    char preserve = main->str[stop];
    main->str[stop] = '\0';

    String returning = string_like(main->str + start);

    main->str[stop] = preserve;

    return returning;
}

/*
    replace all instances of replace in main with with
*/
String replace(String main, String replace, String with){
    /* I hate the code I wrote for this function the most */
    int replaces = count(main, replace);
    int new_size = main->length - replace->length * replaces + with->length * replaces;

    String returning = malloc(sizeof(String));
    returning->str = malloc(new_size + 1);
    returning->str[new_size] = '\0';

    int main_len = main->length;
    char* main_c = main->str;

    int found = find(main, replace);
    int counter = 0;
    int fc = 0;

    while(main->length){
        if(found == fc){
            for(int i = 0;i < with->length;i++, counter++){
                returning->str[counter] = with->str[i];
            }

            main->str += replace->length;
            main->length -= replace->length;

            found = find(main, replace);
            fc = 0;
            continue;
        }
        
        returning->str[counter] = main->str[0];
        counter++, main->length--, main->str++;
    }

    main->length = main_len;
    main->str = main_c;

    return returning;
}

/*
    return index of first occurrence of finding in searching
    -1 if not found
*/
int find(String searching, String finding){
    char* returning_ = strnstr(searching->str, finding->str, searching->length);
    if(returning_){
        return searching->length - strnlen(returning_, searching->length);
    }
    return -1;
}

/*
    return the amount of counting in main
    count(string_like("aaa"), string_like("a")) -> 3
*/
int count(String main, String counting){
    int count = 0;

    char* preserve_c = main->str;
    int preserve_i = main->length;

    int m;

    while(main->length > counting->length){
        if((m = find(main, counting)) != -1){
            main->str += m + counting->length;
            main->length -= counting->length;
            count += 1;
            continue;
        }
        break;
    }

    main->length = preserve_i;
    main->str = preserve_c;

    return count;
}

/*
    compare a to b
    subtracts a by b one character at a time
    returns the first nonzero value or zero if the strings are equal
    if testing for string equality use equals instead for sometimes faster code
    (a - b)
    if positive
        a comes later
*/
int compare(String a, String b){
    int min = a->length < b->length ? a->length : b->length;
    for(int i = 0;i < min;i++){
        int diff = index_without_check(a, i) - index_without_check(b, i);
        if(diff != 0){
            return diff;
        }
    }
    return a->str[min+1] - b->str[min+1]; /* uses a bit of hack since the null terminator is 0 */
}

/*
    returns whether or not two strings are equal
    1 - True, 0 - False
*/
int equals(String a, String b){
    if(a->length != b->length){
        return 0;
    }
    return !compare(a, b);
}

/*
    return a hash for the string 'to_hash'
*/
int hash(String to_hash){
    int returning = 0;

    for(int i = 0;i < to_hash->length;i++){
        returning += ((int) (to_hash->str[i])) * (i + 1);
    }

    return returning;
}

/*
    index without a check for length -- may result in page error
*/
char index_without_check(String str, int index){
    return str->str[index];
}

/*
    get the character of String str at index index, if index is out of range, print an error to stderr and exit(1)
*/
char at_index(String str, int index){
    if(index >= str->length){
        fprintf(stderr, "String index %d is out of range for string \"%s\" of length %d", index, str->str, str->length);
        exit(1);
        return '\0';
    }
    return index_without_check(str, index);
}

/*
    Print a string
    stdio must be loaded
*/
void Print(String a){
    printf("%s", a->str);
}

/*
    Print a string -- newline at end
    stdio must be loaded
*/
void Println(String a){
    printf("%s\n", a->str);
}

void Dln(String a){
    Println(a);
    delete(a);
}

void delete(String to_del){
    free(to_del->str);
    free(to_del);
}

String its(int a){
    char* buffer = malloc(sizeof(1024));
    buffer[1023] = '\0';
    snprintf(buffer, 1023, "%d", a);
    String returning = string_like(buffer);
    free(buffer);
    return returning;
}

String dts(double a){
    char* buffer = malloc(sizeof(1024));
    buffer[1023] = '\0';
    snprintf(buffer, 1023, "%lf", a);
    String returning = string_like(buffer);
    free(buffer);
    return returning;
}

String fts(float a){
    char* buffer = malloc(sizeof(1024));
    buffer[1023] = '\0';
    snprintf(buffer, 1023, "%f", a);
    String returning = string_like(buffer);
    free(buffer);
    return returning;
}

// int main(){
//     Dln(format_string(string_like_burn("%d Days Until %s"), ));
// }