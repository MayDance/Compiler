//
//
//by LBQ
//Reference: Textbook Page 74-82
//All types and functions declared in this header file begin with "Ty_"， Linked list types end with "..list"
//

typedef struct Ty_ty_ *Ty_ty;
typedef struct Ty_tyList_ *Ty_tyList;
typedef struct Ty_fieldList_ *Ty_fieldList;
typedef struct Ty_field_ *Ty_field;
struct Ty_ty_ {
    enum {Ty_int, Ty_float, Ty_char,Ty_array,Ty_struc} kind;  //types
	union {
        Ty_fieldList struc;  //unique types
        Ty_ty array;
    } u;
};
struct Ty_tyList_ {Ty_ty head; Ty_tyList tail;};
struct Ty_fieldList_ {Ty_field head; Ty_fieldList tail;};
struct Ty_field_ {S_symbol name; Ty_ty ty;};
Ty_ty Ty_Int(void);
Ty_ty Ty_Float(void);
Ty_ty Ty_Char(void);

Ty_ty Ty_Struc(Ty_fieldList fields); //type struct
Ty_ty Ty_Array(Ty_ty ty); //type array

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail);
Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail);
