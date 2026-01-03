# RAPPORT FINAL - PSYCHOROBOTS 🤖

## Projet de Simulation Multi-Agents Autonomes avec États Psychologiques

---

## 1. INTRODUCTION

Le projet **PsychoRobots** est une simulation en C++ d'agents robotiques autonomes opérant dans un environnement 2D dynamique. Contrairement aux simulations traditionnelles basées sur des règles déterministes, ce projet introduit des **états psychologiques** (stress, énergie, curiosité) qui influencent les comportements des robots et créent des interactions émergentes complexes.

### Objectifs Principaux
- Simuler le comportement d'agents autonomes avec états psychologiques
- Modéliser l'interaction sociale entre entités hétérogènes
- Créer une architecture extensible pour l'ajout de nouveaux types d'agents
- Implémenter des mécanismes d'apprentissage comportemental rudimentaires

---

## 2. DESCRIPTION DU PROJET

### 2.1 Architecture Générale

Le projet utilise une architecture orientée objet hiérarchisée :

```
Robot (classe abstraite)
├── Worker (Travailleur)
├── Explorer (Explorateur)
└── Social (Robot Social)
```

### 2.2 Types de Robots

#### **Worker (Travailleur)**
- **Objectif primaire** : Compléter des tâches dans l'environnement
- **Attributs** : Stress élevé (50), Énergie élevée (100), Curiosité faible (30)
- **Comportements** :
  - Localise la tâche la plus proche
  - Se déplace vers la tâche
  - Complète la tâche (gagne 10 points)
  - Le stress **diminue de 15** après chaque tâche complétée
  - La curiosité **augmente de 5** après completion
  - Résigne quand l'énergie atteint 0 ou toutes les tâches sont complétées
- **Psychologie** : Représente un agent orienté tâche avec satisfaction par l'accomplissement

#### **Explorer (Explorateur)**
- **Objectif primaire** : Découvrir et visiter toutes les zones d'exploration
- **Attributs** : Stress modéré (30), Énergie très élevée (90), Curiosité très élevée (90)
- **Comportements** :
  - Identifie les zones non visitées à proximité
  - Se déplace et visite les zones
  - Accumule les connaissances sur les zones
  - Perd 10 points de curiosité après avoir exploré toutes les zones
  - Quitte la simulation quand la curiosité atteint 0
- **Psychologie** : Représente un agent d'exploration avec désir de découverte qui s'atténue avec la satisfaction

#### **Social (Robot Social)**
- **Objectif primaire** : Interagir avec d'autres robots pour diminuer le stress
- **Attributs** : Stress faible (20), Énergie modérée (80), Curiosité modérée (60)
- **Comportements** :
  - Analyse l'environnement (rayon de détection : 10 unités)
  - Initie des appels (CALL/RESPONSE protocol)
  - Sélectionne les cibles selon :
    - Priorité aux robots sociaux
    - Proximité (pour économiser l'énergie)
    - Énergie disponible
    - Niveau de curiosité
  - Le stress **diminue de 5** après interaction réussie
  - L'énergie **augmente de 3** après interaction
- **Psychologie** : Représente un agent social avec dépendance aux interactions pour la régulation du stress

### 2.3 Mécaniques de Simulation

#### **Gestion de l'Énergie**
- Chaque mouvement coûte 2 unités d'énergie
- Repos automatique quand énergie < 20
- Le repos restaure 2 unités d'énergie par cycle
- Plage valide : [0, 100]

#### **Statut des Robots**
- **Free** : Disponible pour mouvement/interaction
- **InCall** : Engagé dans une interaction
- **Busy** : Ne peut pas accepter d'interactions

#### **Protocole CALL/RESPONSE**
1. Robot Social initie un appel
2. Robot cible répond selon ses critères :
   - Curiosité > 40
   - Stress < 60
   - Énergie > 30
3. Si accepté : interaction commence
4. Si rejeté : attendre avant nouvelle tentative

---

## 3. PROBLÈMES RENCONTRÉS ET SOLUTIONS

### 3.1 Problème 1 : Attributs Négatifs des Robots

**Problème** : Après plusieurs cycles, les attributs (curiosité, stress, énergie) pouvaient devenir négatifs (ex: -90), ce qui n'avait pas de sens physique.

**Solution Implémentée** :
```cpp
// Dans robot.hpp - Setters avec clamping
void set_stress(int s) {stress = std::max(0, std::min(100, s));}
void set_energy(int e) {energy = std::max(0, std::min(100, e));}
void set_curiosity(int c) {curiosity = std::max(0, std::min(100, c));}
```

**Impact** : Tous les attributs sont maintenant bornés dans [0, 100], ce qui maintient la cohérence psychologique.

---

### 3.2 Problème 2 : Fin de Simulation Prématurée

**Problème** : La simulation se terminait trop tôt quand les explorateurs avaient une curiosité faible, même s'il y avait encore des activités en cours.

**Problème Spécifique** : Les messages "l'explorateur veut partir" s'affichaient à chaque frame, et les conditions de fin étaient trop laxes.

**Solution Implémentée** :
- Ajout d'un compteur `leaveAttempts` pour tracker l'engagement à partir
- Seul le premier appel est affiché (pas de spam)
- Conditions strictes pour vouloir partir :
  - `wantsNewEnvironment == true` (zones explorées)
  - `curiosity == 0` (complètement saturé) OU (`curiosity < 10` ET `energy < 30`)
- Minimum 10 étapes de simulation requis

```cpp
// Explorer::update()
if (wantsNewEnvironment && get_curiosity() == 0) {
    leaveAttempts++;
    if (leaveAttempts == 1)  // Affiche une seule fois
        std::cout << "Explorer " << r_id << " curiosity depleted. Wants to leave.\n";
}
```

**Impact** : Les simulations durent maintenant plus longtemps et se terminent de manière appropriée.

---

### 3.3 Problème 3 : Cas Limites (Edge Cases)

**Problème** : Quand la simulation contenait :
- **Que des Travailleurs** : Pas de détection de la fin (tous les travaux finis)
- **Que des Explorateurs** : Pas de détection de la fin (toutes les zones visitées)

**Solution Implémentée** : Détection intelligente basée sur la composition :

**Cas 1 - Travailleurs Uniquement** :
```cpp
if (allTasksCompleted && totalWorkers > 0) {
    for (Worker* w : workers) {
        w->resign();  // Forcer la résignation
    }
}
```

**Cas 2 - Explorateurs Uniquement** :
```cpp
if (allZonesExplored && totalExplorers > 0) {
    for (Explorer* e : explorers) {
        e->set_wantsNewEnvironment(true);
        e->set_leaveAttempts(1);
    }
}
```

**Cas 3 - Robots Mixtes** : Les trois conditions doivent être satisfaites :
- Tous les travailleurs résignés
- Tous les explorateurs veulent partir
- Tous les robots sociaux isolés (0 interactions)

**Impact** : Le système fonctionne maintenant correctement pour tous les scénarios de composition.

---

### 3.4 Problème 4 : Manque de Détection de Fin dans Cas Mixte

**Problème** : Avec des travailleurs ET des explorateurs, les travailleurs continuaient à chercher des tâches même quand tout était complété, empêchant la fin de la simulation.

**Solution Implémentée** : Vérification active dans `checkSimulationComplete()` :
```cpp
if (allTasksCompleted && totalWorkers > 0) {
    for (Robot* r : this->robots) {
        Worker* w = dynamic_cast<Worker*>(r);
        if (w && !w->get_hasResigned()) {
            w->resign();  // Forcer résignation quand tous les travaux finis
        }
    }
}
```

**Impact** : Les travailleurs se résignent automatiquement quand toutes les tâches sont complétées.

---

## 4. ARCHITECTURE IMPLÉMENTÉE

### 4.1 Système de Complétion

La simulation utilise un système de détection à trois niveaux :

```
checkSimulationComplete()
├─ Déterminer composition des robots
├─ Appliquer règles selon composition
└─ Retourner true/false
```

### 4.2 Points Psychologiques Clés

1. **Satisfaction par Accomplissement** (Workers)
   - Stress diminue après tâches
   - Crée motivation intrinsèque à continuer

2. **Saturation Exploratoire** (Explorers)
   - Curiosité diminue quand exploration complète
   - Quand curiosité = 0 → désir de quitter
   - Simule l'ennui et la nécessité de changement

3. **Régulation du Stress Social** (Social)
   - Les interactions réduisent le stress
   - Crée une dépendance sociale
   - Simule le besoin humain de connexion

---

## 5. SOLUTIONS IMPLÉMENTÉES - RÉSUMÉ

| Problème | Solution | Résultat |
|----------|----------|----------|
| Attributs négatifs | Clamping [0,100] | Cohérence garantie |
| Fin prématurée | Conditions strictes + leaveAttempts | Simulations plus longues |
| Cas limites ignorés | Logique conditionnelle par composition | Tous les scénarios couverts |
| Workers ne résignent pas | Détection tâches complétées | Fin appropriée |
| Messages spammés | Compteur d'affichage une fois | Interface claire |

---

## 6. FONCTIONNALITÉS FUTURES - VISION AMBITIEUSE

### 6.1 Intelligence Artificielle

#### **Apprentissage par Renforcement**
- Implémenter Q-Learning pour optimiser les stratégies de robots
- Permettre aux robots d'apprendre les chemins optimaux
- Améliorer les critères de sélection d'interaction

#### **Algorithmes Génétiques**
- Évolution de populations de robots
- Sélection naturelle basée sur performance
- Mutation des traits comportementaux

#### **Réseaux de Neurones**
- Prédire les meilleures actions selon l'état
- Créer des neurones décisionnels pour navigation
- Apprentissage des patterns d'interaction

### 6.2 Psychologie Avancée

#### **Modèle Psychologique Enrichi**
- Ajouter **Peur** (évite dangers)
- Ajouter **Confiance** (dans les autres robots)
- Ajouter **Frustration** (quand objectifs bloqués)
- Ajouter **Empathie** (aide les autres robots)

#### **Personnalités de Robots**
- Introvertis vs Extravertis
- Pessimistes vs Optimistes
- Agressifs vs Pacifiques
- Leader vs Followers

#### **Mémoire Émotionnelle**
- Les robots se souviennent d'interactions positives/négatives
- Influencent les décisions futures
- Créent des relations durables

### 6.3 Nouveaux Types de Robots

- **Soldier** : Patrouille, protège d'autres robots
- **Trader** : Échange de ressources, commerce
- **Teacher** : Partage connaissances, transfert d'apprentissage
- **Scout** : Exploration rapide, faible énergie
- **Healer** : Réduit stress des autres robots
- **Leader** : Coordonne groupes de robots

### 6.4 Environnement Dynamique

#### **Obstacles et Terrains**
- Obstacles statiques/dynamiques
- Terrains différents (prairie, désert, montagne)
- Coûts énergétiques variables selon terrain
- Pluie, neige, tempêtes affectant la vision

#### **Ressources et Économie**
- Ressources à collecter
- Monnaie/Points à échanger
- Marchés et trading
- Entreprises qui engagent des Workers
- Salaires et contrats de travail
- Compétences et progression de carrière

### 6.5 Visualisation Graphique

#### **2D Avancée**
- SDL2 pour rendu temps réel
- Animation fluide des mouvements
- Particules et effets visuels
- Interface utilisateur interactive

#### **3D Immersive**
- OpenGL pour environnement 3D
- Caméra contrôlable
- Éclairage dynamique
- Modèles de robots détaillés

#### **Réalité Augmentée**
- Affichage d'informations en AR
- Suivi des robots en temps réel
- Statistiques superposées

### 6.6 Système de Communication

#### **Langage de Robots**
- Protocole de communication texte
- Syntaxe définie pour messages
- Broadcasting et point-to-point
- Gossip network (rumeurs)

#### **Partage d'Information**
- Workers partagent tâches complétées
- Explorers partagent zones découvertes
- Socials partagent statut/besoin
- Information decay (vieillit avec le temps)

### 6.7 Activités Élargies

#### **Coopération**
- Tâches nécessitant plusieurs robots
- Bonus coopération
- Formations de groupes

#### **Compétition**
- Ressources limitées
- Conflits pour objectifs
- Hiérarchies et statuts

#### **Évènements Dynamiques**
- Catastrophes (tremblements, inondations)
- Crises testant la coopération
- Objets rares à trouver

### 6.8 Persistance et Analytics

- Sauvegarde/chargement d'états
- Enregistrement de statistiques
- Graphiques de performances
- Analyse comportementale post-simulation
- Export de données pour machine learning

---

## 7. INNOVATION PÉDAGOGIQUE

Ce projet transcende le cadre académique traditionnel en introduisant :

### **Concepts Théoriques Implémentés**
- Systèmes multi-agents autonomes
- Comportements émergents
- Théorie des jeux (interactions)
- Psychologie computationnelle
- Intelligence artificielle distribuée

### **Application Pratique**
- Architecture OOP en C++ moderne
- Design patterns (Factory, Observer, Strategy)
- Gestion d'états complexes
- Optimisation de performance
- Débogage de comportements complexes

### **Aspects Interdisciplinaires**
- Informatique (algorithmes, structures)
- Psychologie (états émotionnels, comportements)
- Économie (trading, ressources)
- Sociologie (interactions, groupes)
- Philosophie (libre arbitre, décisions)

---

## 8. CONCLUSION

Le projet **PsychoRobots** démontre qu'une simulation complexe peut émerger de règles simples lorsque des **états psychologiques** sont appliqués à des agents autonomes. Chaque problème rencontré a mené à une solution qui améliore non seulement la robustesse du système mais aussi sa cohérence conceptuelle.

Les futures développements envisagés positionnent ce projet comme une plateforme complète pour :
- Tester des théories de psychologie computationnelle
- Développer et valider des algorithmes d'IA
- Créer une simulation immersive d'écosystèmes multi-agents
- Éduquer sur les systèmes complexes et les comportements émergents

Ce n'est pas simplement un projet universitaire, mais le fondement d'une plateforme de recherche en agents autonomes intelligents.

---

## 9. RÉFÉRENCES TECHNIQUES

### Fichiers Clés
- `include/robot.hpp` - Classe de base abstraite
- `src/worker.cpp` - Logique des travailleurs (task completion, resignation)
- `src/explorer.cpp` - Logique d'exploration et de départ
- `src/simulation.cpp` - Détection de completion et états globaux
- `include/common_types.hpp` - Constantes et énumérations

### Constantes Principales
- Grille : 30x30
- Tâches par travailleur : 3
- Zones par explorateur : 2
- Distance d'interaction : 1.0
- Étapes minimum : 10

---

**Date** : 3 Janvier 2026  
**Projet** : PsychoRobots - Simulation Multi-Agents avec États Psychologiques  
**Langage** : C++ 11+  
**Status** : En développement actif  
**Version** : 1.0 (Core Features Complete)
