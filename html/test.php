<?php
// Récupérer les paramètres de la ligne de commande
// $options = getopt("", ["name:", "age:"]);
// $name = isset($options['name']) ? $options['name'] : '';
// $age = isset($options['age']) ? $options['age'] : '';
$name = getenv('NAME');
$age = getenv('AGE');

// Effectuer une vérification de base sur les paramètres
if (empty($name) || empty($age)) {
    echo "<html>";
    echo "Erreur: Veuillez fournir un nom et un âge valides.";
    echo "</html>";
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
echo "<html>";
echo "<head>";
echo "<title>Resulat du script </title>";
echo "</head>";
echo "<body>";
echo "<h1>Réponse du script PHP</h1>";
echo "<p>$greeting</p>";
echo "</body>";
echo "</html>";
?>
