<?php
// Récupérer les paramètres de l'environnement
$name = getenv('NAME');
$age = getenv('AGE');

// Effectuer une vérification de base sur les paramètres
if (empty($name) || empty($age)) {
    echo "<html>";
    echo "<head>";
    echo "<title>Erreur</title>";
    echo "</head>";
    echo "<body>";
    echo "<h1>Erreur: Veuillez fournir un nom et un age valides.</h1>";
    echo "</body>";
    echo "</html>";
    exit;
}

// Effectuer une action basée sur les paramètres
$greeting = "Bonjour, $name! ";
if ($age < 18) {
    $greeting .= "Vous etes mineur.";
} else {
    $greeting .= "Vous etes majeur.";
}

// Générer une réponse HTML
echo "<html>";
echo "<head>";
echo "<title>Resultat du script</title>";
echo "</head>";
echo "<body>";
echo "<h1>Reponse du script PHP</h1>";
echo "<p>$greeting</p>";
echo "</body>";
echo "</html>";
?>

