function get_image() {
    var img = document.createElement('img');
    img.src = 
    'http:localhost:5000/api/result';

    document.getElementById('body').appendChild(img);
    down.innerHTML = "Image Element Added.";
}