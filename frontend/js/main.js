$(document).ready(function() {
    'use strict'; // Prevents certain (unsafe) actions from being taken and throws more exceptions.
    const ALERT_ELEMENT = '#alert';
    const MAILS_ELEMENT = '#mails';
    const URL = 'http://172.20.10.4/arduino/digital/';
    const MILLISECONDS = 15000;
    const TITLE = 'Snail Mail Notification System';

    function getMails() {
        $.getJSON(URL, function(data, status){
            toggleElements(data.mails);
        });
    }

    function toggleElements(mails) {
        $(MAILS_ELEMENT).text('Snail mails in your mailbox: ' + mails + ' ');
        if(mails > 0) {
            $(ALERT_ELEMENT).removeClass('hidden');
            document.title = '(' + mails + ') ' + TITLE;
        }
        else {
            document.title = TITLE;
            $(ALERT_ELEMENT).addClass('hidden');
        }
    }

    setInterval(getMails, MILLISECONDS);
});
