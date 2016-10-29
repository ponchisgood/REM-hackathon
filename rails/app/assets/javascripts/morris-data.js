// Morris.js Charts sample data for SB Admin template

$(function() {

    Morris.Line({
        element: 'user-chart',
        data: [
        { d: '2016-10-22', movie: 100, med: 90, shopping: 24, beer: 64, computer: 37 },
        { d: '2016-10-23', movie: 23, med: 41, shopping: 12, beer: 12, computer: 12 },
        { d: '2016-10-24', movie: 54, med: 23, shopping: 43, beer: 34, computer: 32 },
        { d: '2016-10-25', movie: 51, med: 52, shopping: 53, beer: 34, computer: 45 },
        { d: '2016-10-26', movie: 75, med: 66, shopping: 22, beer: 65, computer: 45 },
        { d: '2016-10-27', movie: 81, med: 84, shopping: 45, beer: 67, computer: 23 },
        { d: '2016-10-28', movie: 23, med: 11, shopping: 46, beer: 89, computer: 12 },
        { d: '2016-10-29', movie: 35, med: 9, shopping: 33, beer: 75, computer: 32 }
        ],
        xkey: 'd',
        ykeys: ['movie','med','shopping','beer','computer'],
        labels: ['Movie','Medicine','Shopping','Beer','Computer']
    });
});
