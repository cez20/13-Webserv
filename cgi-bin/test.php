?php
    // Récupérer les paramètres de requête
    $name = $_GET['name'];
    $age = $_GET['age'];

    // Effectuer une vérification de base sur les paramètres
    if (empty($name) || empty($age)) {
        echo "Content-Type: text/html\r\n\r\n";
        echo "Erreur: Veuillez fournir un nom et un âge valides.";
        exit;
    }

    // Effectuer une action basée sur les paramètres
    $greeting = "Bonjour, $name! ";
    if ($age < 18) {
        $greeting .= "Vous êtes mineur.";
    } else {
        $greeting .= "Vous êtes majeur.";
    }

    // Générer une réponse HTML
    echo "Content-Type: text/html\r\n\r\n";
    echo "<h1>Réponse du script PHP</h1>";
    echo "<p>$greeting</p>";
?>