var _users = []
var _cardRecieveds = []
var _fileName = '/home/pi/cardRecieveds.txt'

function dateStrToDate(dt) {
    var year = +dt.substr(0, 4);
    var month = +dt.substr(4, 2) - 1;
    var day = +dt.substr(6, 2);
    var hour = +dt.substr(8, 2);
    var minute = +dt.substr(10, 2);
    var second = +dt.substr(12, 2);
    //console.log(year, month, day, hour, minute, second)
    return new Date(year, month, day, hour, minute, second)
}

function makeDate(currentValue, index, array) {
    var dt = array[index].lastCardTime;    
    if(dt !== null) {        
        array[index].lastCardTime = dateStrToDate(dt)
    }
//    var a = /\/Date\((\d*)\)\//.exec(dt);
//    if (a) {
//        array[index].lastCardTime = new Date(+a[1]);  //+a[1] -> converts a[1] to number
//    }
}

function makeDateCardRec(currentValue, index, array) {
    array[index].datetime = new Date(array[index].datetime);
}

function getAllUsers(json, forceGetPhotos) {
    if(forceGetPhotos === undefined)
        forceGetPhotos = false
    var res = JSON.parse(json);
    if(res.success === 'true') {
        _users = res.persons
        _users.forEach(makeDate);
        progressBar.visible = true
        //console.log(JSON.stringify(_users))
        //console.log(_users[0].lastCardTime.toLocaleString(Qt.locale('en_US'), 'HH:mm:ss'))
        userOperate.getUsersPhoto(_users, forceGetPhotos)
        var cards = userOperate.readFile(_fileName);
        if(cards !== '') {
            if(cards === '[]')
                _cardRecieveds = []
            else {
                _cardRecieveds = JSON.parse(cards)
                _cardRecieveds.forEach(makeDateCardRec);
                //console.log(JSON.stringify(_cardRecieveds))
                var revCardsRec = _cardRecieveds.slice().reverse();
                for(var i = 0; i < _users.length; i++) {
                    var index = revCardsRec.findIndex(function(value) {
                        return value.id === _users[i].id
                    });
                    if(index >= 0) {
                        _users[i].lastCardTime = revCardsRec[index].datetime
                    }
                }
                //console.log(JSON.stringify(_users))
            }
        }

    } else
        console.log('error in reading users' + res.message)
}

function cardRecieved(cardNo) {
    //console.log(JSON.stringify(_users))
    var index = _users.findIndex(function (value) {
        return value.CardNo === cardNo
    });
    if(index >= 0) {
        var user = _users[index];
        console.log(JSON.stringify(user))
        var date = new Date();
        var lastCardtime;
        if(user.lastCardTime !== null)
            lastCardtime = new Date(user.lastCardTime.getTime() + 600000) //add 10 minutes
        else
            lastCardtime = new Date(2000, 1, 1)
        if(date > lastCardtime) {
            showUser.show(user, false)
            _cardRecieveds.push({id: user.id, datetime: date})
            _users[index].lastCardTime = date            
            saveCardsToFile()
        } else {
            showUser.show(user, true)
        }
    }
    else {
        console.log('card not found')
    }
}

function saveCardsToFile() {
    //console.log(JSON.stringify(_cardRecieveds))
    userOperate.saveFile(_fileName, JSON.stringify(_cardRecieveds))
}

function addCardRecieveds() {
    if(_cardRecieveds.length > 0 && props.restApiType === root.restFunctions.none) {
        props.restApiType = root.restFunctions.addCardRead
        restApi.startGet("AddToList", makeAddToListArgs());
        busy.visible = true
    }
}

function makeAddToListArgs() {
    var ids = ''
    var dateTimes = ''
    _cardRecieveds.forEach(function (value){
        //console.log(JSON.stringify(value))
        ids = ids + value.id + ','
        dateTimes = dateTimes + value.datetime.toLocaleString(Qt.locale('en_US'), 'yyyyMMddHHmmss') + ','
    })
    ids = ids.substring(0, ids.length - 1)
    dateTimes = dateTimes.substring(0, dateTimes.length - 1)
    return 'id=' + ids + '&datetime=' + dateTimes
}
