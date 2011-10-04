importPackage(java.io);
importPackage(java.lang);

var points = { '0':0, '1':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9, '/':10, 'X':10 };

var reader = new BufferedReader(new InputStreamReader( new FileInputStream(arguments[0]), "UTF8" )); 
if (reader) {
    var line;
    while (line = reader.readLine()) {
        var marks = line.split(' ');
        var score = 0;
        var ball = 0;
        var frames = 10;
        while (frames--) {
            if (marks[ball] == 'X') {
                ball++;
                score += ((marks[ball+1] == '/') ? 20 : 10 + points[marks[ball]] + points[marks[ball+1]]);
            } else {
                score += ((marks[ball+1] == '/') ? 10 + points[marks[ball+2]] : points[marks[ball]] + points[marks[ball+1]]);
                ball += 2;
            }

        }

        print(score);
    }
    reader.close();
}

