.pragma library

function zeroPadding(num, count) {
    var str = "";
    for(var i = 0; i < count; i++)
        str += "0";
    return (str + num).slice(-count);
}

function spacePadding(num, count) {
    var str = "";
    for(var i = 0; i < count; i++)
        str += " ";
    return (str + num).slice(-count);
}

function convert(a){
    return ['۰', '۱', '۲', '۳', '۴', '۵', '۶', '۷', '۸', '۹'][a];
}
function convertToPersian(txt){
    return txt.replace(/\d/g, convert);
}
