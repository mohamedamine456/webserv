<?php
    $cookie_name = "user";
    $cookie_value = "Mohamed Amine";
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
    // $cookie_name = "project";
    // $cookie_value = "WebServ";
    // setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
    // $cookie_name = "task";
    // $cookie_value = "Cookizat";
    // setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
    
    
?>
<html>
    <body>
        <?php
            if(!isset($_COOKIE["user"])) {
                echo "Cookie named user is not set!";
            } else if (!isset($_COOKIE["project"])) {
                echo "Cookie named project is not set!";
            } else if (!isset($_COOKIE["task"])) {
                echo "Cookie named task is not set!";
            } else {
                echo "Welcome to <strong>" . $_COOKIE["project"] . "</strong> task of <u>" . $_COOKIE["task"] . '</u> Done by: <strike>' . $_COOKIE["user"] . "</strike>";
            }
        ?>
    </body>
</html>