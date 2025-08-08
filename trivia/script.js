// Track user's score
let score = 0;
const totalQuestions = 2;

// Track which questions have been answered
const answered = {
    multipleChoice: false,
    freeResponse: false
};

// DOM elements
const choices = document.querySelectorAll('.choice');
const freeAnswerInput = document.getElementById('free-answer');
const submitButton = document.getElementById('submit-answer');
const feedback1 = document.getElementById('feedback1');
const feedback2 = document.getElementById('feedback2');
const scoreElement = document.getElementById('score');

// Function to check multiple choice answers
function checkMultipleChoice(selectedButton) {
    // Only allow answering once
    if (answered.multipleChoice) return;

    // Get the user's selection
    const userAnswer = selectedButton.getAttribute('data-answer');
    const correctAnswer = 'mars';

    if (userAnswer === correctAnswer) {
        // Correct answer
        selectedButton.classList.add('correct-bg');
        feedback1.textContent = 'Correct! Mars is indeed the Red Planet.';
        feedback1.className = 'feedback correct';
        score++;
    } else {
        // Incorrect answer
        selectedButton.classList.add('incorrect-bg');
        feedback1.textContent = 'Incorrect. The correct answer is Mars.';
        feedback1.className = 'feedback incorrect';

        // Highlight correct answer
        document.querySelector('[data-answer="mars"]').classList.add('correct-bg');
    }

    // Update score and mark as answered
    answered.multipleChoice = true;
    updateScore();
}

// Function to check free response answers
function checkFreeResponse() {
    // Only allow answering once
    if (answered.freeResponse) return;

    // Get and clean user input
    const userAnswer = freeAnswerInput.value.trim().toLowerCase();
    const correctAnswers = ['blue whale', 'the blue whale'];

    if (correctAnswers.includes(userAnswer)) {
        // Correct answer
        freeAnswerInput.classList.add('correct-bg');
        feedback2.textContent = 'Correct! The Blue Whale is the largest mammal.';
        feedback2.className = 'feedback correct';
        score++;
    } else {
        // Incorrect answer
        freeAnswerInput.classList.add('incorrect-bg');
        feedback2.textContent = 'Incorrect. The correct answer is the Blue Whale.';
        feedback2.className = 'feedback incorrect';
    }

    // Update score and mark as answered
    answered.freeResponse = true;
    updateScore();
}

// Function to update the score display
function updateScore() {
    scoreElement.textContent = score;
}

// Add event listeners to multiple choice buttons
choices.forEach(button => {
    button.addEventListener('click', () => {
        checkMultipleChoice(button);
    });
});

// Add event listener to submit button
submitButton.addEventListener('click', checkFreeResponse);

// Add event listener for Enter key in text input
freeAnswerInput.addEventListener('keyup', (event) => {
    if (event.key === 'Enter') {
        checkFreeResponse();
    }
});
