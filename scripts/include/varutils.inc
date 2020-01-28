///////////////////////////////////////////////////////////////////////////
// varutil -- more commonly used variable processing functions

// Make array entries into numbers
Function CIntArray(byref arr) // {{{
  var newarr := array{};

  ForEach elem in (arr)
    SleepMs(2);
    newarr.append(CInt(elem));
  EndForEach

  return newarr;
EndFunction // }}}

// wandelt Strings der Form "0x1234, 0x3434-0x3466, 123, 10-15" in Array
// from numbers, whereby all intermediate values ​​are generated at <from> - <bis>
Function ResolveRange(unresolved) // {{{
    If (TypeOfInt(unresolved) != OT_STRING)
        return unresolved;
    EndIf

    unresolved:=StrReplace(unresolved," ","");

    var resolved := array{};
    ForEach word in (SplitWords(unresolved, ","))
        If (word["-"])
            var delims := SplitWords(word, "-");
            var elem;
            For (elem := CInt(delims[1]); elem <= CInt(delims[2]); elem += 1)
                resolved.append(elem);
            EndFor
        Else
            resolved.append(CInt(word));
        EndIf
        SleepMS(2);
    EndForEach

    return resolved;
EndFunction // }}}

// Funktion zum klassenbasierten Zerhacken von Strings
// Beispiel: SplitClasses(
//      "Du dumme Sau!^^", array{"ABC...XYZabc...xyz", " !?."}
//      ) wird zu:
//      array{"Du", " ", "dumme", " ", "Sau", "!", "^^"}
//
// Oder doch besser ein Dictionary mit Verweis, welche Klasse aus dem
// Array gepasst hat?
// Function SplitClasses(string, class_array)
//     return;
// EndFunction

// Convert array to string with separator <delim>
Function Join(delim, arr) // {{{
    If (arr.size() <= 0)
        return "";
    EndIf

    var result := arr[1];

    var i;
    For (i := 2; i <= arr.size(); i += 1)
        result += delim + arr[i];
    EndFor

    return result;
EndFunction // }}}

// removes duplicates from array (note: sorts array)
Function Unique(non_unique_arr) // {{{
    var unique_arr := array{};
    If (!non_unique_arr or non_unique_arr.size() == 0)
        return unique_arr;
    EndIf

    non_unique_arr.sort();

    var last := non_unique_arr[1];
    unique_arr.append(last);
    non_unique_arr.erase(1);
    ForEach entry in (non_unique_arr)
        SleepMS(2);
        If (entry != last)
            unique_arr.append(entry);
        EndIf
        last := entry;
    EndForEach

    return unique_arr;
EndFunction // }}}

//exchanges values ​​of two variables
Function SwapValues(byref val1, byref val2) // {{{
    // TODO: bei Integer geht es ohne Hilfsvariable
    var help := val1;
    val1 := val2;
    val2 := help;
EndFunction // }}}

// turns the option string into the form "foo = bar", "baz", "nobar"
// dict {foo -> bar, baz -> 1, nobar -> 1, bar -> 0}
Function GetOptions(optstr) // {{{
    var params := SplitWords(optstr);

    var result := dictionary;
    ForEach elem in (params)
        SleepMs(2);
        If (elem["="])
// hack complex options
            var keyvalue := SplitWords(elem, "=");
            result.insert(keyvalue[1], keyvalue[2]);
            continue;
        EndIf

        // Rest normal einfuegen
        result.insert(elem, 1);

        // if the options are negated, add a variant without "no"
        If (Find(elem, "no", 0) == 1)
            result.insert(SubStr(elem, 3, Len(elem) - 2), 0);
        EndIf
    EndForEach

    return result;
EndFunction // }}}

// Target double conversion to decimal notation (string)
Function PrettyPrintDouble(dbl) // {{{
    dbl := CStr(dbl);

    If (!(dbl["+"]))
        return dbl;
    EndIf

// separate separated decimal places
    var parts := SplitWords(dbl, "e+");
    var zeros := CInt(parts[2]);
    dbl       := parts[1];

    If (!(dbl["."]))
       // Integer, just add enough zeros
        For i := 1 To zeros
            dbl += "0";
        EndFor
        return dbl;
    EndIf

    parts := SplitWords(dbl, ".");
    // Punkt entfernen
    var missingzeros := zeros - Len(parts[2]);
    If (missingzeros < 0)
        // ich zweifele, dass es je auftritt
        syslog(
            "WARNUNG: "+dbl+" mit "+zeros+" Exponentialstellen hat "+
            missingzeros+" fehlende Nullen"
        );
    EndIf
    dbl["."] := "";
    For i := 1 To missingzeros
        // nur fehlende Nullen ergaenzen
        dbl += "0";
    EndFor

    return dbl;
EndFunction // }}}

// more beautiful numbers with suitable prefixes (add unit yourself)
Function PrettyPrintPrefix(dbl, unit := "", base := 1000, fracs := 1) // {{{
    var binar := "";
    If (CInt(base) == 1024)
        binar := "i";
    EndIf
    var neg := "";
    If (dbl < 0)
        neg := "-";
        dbl := -1 * dbl;
    EndIf

    var org := dbl;
    base    := CDbl(base);

    // well, not nice, but at least rarely
    ForEach prefix in (array{"", "k", "M", "G", "T", "P", "E", "Z", "Y"})
        If (dbl >= base)
            dbl := dbl / base;
        Else
           // roughly round to suitable decimal places
            dbl := CInt(dbl * Pow(10, fracs)) / Pow(10, fracs);

            If (prefix == "")
                // no prefix, output directly
                If (unit == "")
                    // no unit, no space
                    return neg+CStr(dbl);
                EndIf
                // separate unit with spaces
                return neg+CStr(dbl)+" "+unit;
            EndIf

            // Prefix und ggf. Binaerkennung anhaengen
            return neg+CStr(dbl)+" "+prefix+binar+unit;
        EndIf
    EndForEach

    // way too big (unlikely)
    return org;
EndFunction // }}}

// cheaper, self-made ternary operator
Function Tern(bool, iftrue, iffalse) // {{{
    If (bool)
        return iftrue;
    EndIf

    return iffalse;
EndFunction // }}}

// generates a random code that differs from dont_accept (if set)
Const RANDOM_CODE_LENGTH := 8;
Function GenRandomCode(dont_accept := 0) // {{{
    // The following characters are used for the code
    // alphanumeric, except o, O, 0 and i, I, l due to confusion
    var chars := {
        "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F",
        "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
        "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k",
        "m", "n", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
    };

    var code := "";
    While (Len(code) < RANDOM_CODE_LENGTH)
        code += chars.randomentry();
        If (dont_accept and code == dont_accept)
            code := "";
        EndIf
    EndWhile

    return code;
EndFunction // }}}

// turns string <str> into a string array with maximum line length <wrap>
Function TextWrap(str, wrap := 60) // {{{
    var lines := array{};
    var line  := "";
    ForEach word in (SplitWords(str))
        SleepMs(2);
        If (Len(line+word+" ") <= wrap)
            // always attach as long as it fits
            line += word+" ";
        Else
            // doesn't fit anymore, start a new line
            lines.append(Trim(line));
            line := word+" ";
        EndIf
    EndForEach
    // anything else?
    If (line)
        lines.append(Trim(line));
    EndIf

    return lines;
EndFunction // }}}

// Make the first character of the string uppercase
Function UCFirst(str)
    str[1] := Upper(str[1]);
    return str;
EndFunction
