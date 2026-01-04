#!/bin/bash
# Script pour compiler l'AI Gomoku en exécutable Windows (.exe)

set -e

echo "🔨 Compilation de l'AI Gomoku pour Windows..."
echo ""

# Vérifier que MinGW est installé
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "❌ Erreur: MinGW n'est pas installé"
    echo "   Installez-le avec: sudo dnf install mingw64-gcc-c++"
    exit 1
fi

# Se placer dans le répertoire du projet
cd "$(dirname "$0")"

# Compiler pour Windows
echo "📦 Compilation avec MinGW..."
x86_64-w64-mingw32-g++ \
    -std=c++20 \
    -O3 \
    -static \
    -o pbrain-gomoku-ai.exe \
    src/*.cpp \
    -Iinclude

# Vérifier que la compilation a réussi
if [ -f "pbrain-gomoku-ai.exe" ]; then
    SIZE=$(du -h pbrain-gomoku-ai.exe | cut -f1)
    echo ""
    echo "✅ Compilation réussie !"
    echo "   📁 Fichier: pbrain-gomoku-ai.exe"
    echo "   📏 Taille: $SIZE"
    echo ""
    echo "🧪 Test de l'exécutable..."
    
    # Tester avec Wine
    if command -v wine &> /dev/null; then
        RESULT=$(printf "START 20\nBEGIN\nEND\n" | wine pbrain-gomoku-ai.exe 2>&1 | grep -v "fixme\|wine-staging" | head -2)
        if echo "$RESULT" | grep -q "OK"; then
            echo "   ✅ Test réussi - L'AI répond correctement"
        else
            echo "   ⚠️  L'AI ne répond pas comme attendu"
        fi
    else
        echo "   ℹ️  Wine n'est pas installé, impossible de tester"
    fi
    
    echo ""
    echo "🎮 Utilisez ce fichier dans Piskvork:"
    echo "   $(pwd)/pbrain-gomoku-ai.exe"
else
    echo ""
    echo "❌ Erreur: La compilation a échoué"
    exit 1
fi
