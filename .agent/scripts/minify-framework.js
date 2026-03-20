const fs = require('fs');
const path = require('path');

// Simple script to minify `.agent` markdown files to reduce token bloat for CLI workers
// Use when packaging or before large swarm workflows.

const args = process.argv.slice(2);
const shouldWrite = args.includes('--write');

if (!shouldWrite) {
  console.log('DRY RUN MODE: Pass --write to actually modify files.');
}

const AGENT_DIR = path.resolve(__dirname, '..');

function minifyMarkdown(content) {
  // 1. Remove excessive empty lines (more than 2 consecutive newlines -> 2)
  let minified = content.replace(/\n{3,}/g, '\n\n');
  
  // 2. Remove HTML comments <!-- ... --> EXCEPT <!-- slide --> (for carousels)
  minified = minified.replace(/<!--(?!\s*slide\s*-->)[\s\S]*?-->/g, '');
  
  // 3. Trim trailing whitespace but preserve markdown hard breaks (two spaces)
  minified = minified.split('\n').map(line => {
    const hasHardBreak = line.endsWith('  ');
    return line.trimEnd() + (hasHardBreak ? '  ' : '');
  }).join('\n');
  
  // 4. (Optional) Remove markdown blockquotes that are purely fluff.
  // We skip this to avoid breaking functional info blockquotes like > Note needed for context.
  
  return minified.trim() + '\n';
}

function traverseAndMinify(dir) {
  const files = fs.readdirSync(dir);
  for (const file of files) {
    if (file === 'spawn_agent_tasks' || file === 'benchmarks' || file === '.hc' || file === '.git' || file === 'node_modules') continue;
    
    const fullPath = path.join(dir, file);
    if (fs.statSync(fullPath).isDirectory()) {
      traverseAndMinify(fullPath);
    } else if (file.endsWith('.md')) {
      const original = fs.readFileSync(fullPath, 'utf8');
      const minified = minifyMarkdown(original);
      if (original !== minified) {
        if (shouldWrite) {
          fs.writeFileSync(fullPath, minified, 'utf8');
          console.log(`Minified: ${path.relative(AGENT_DIR, fullPath)}`);
        } else {
          console.log(`[DRY RUN] Would minify: ${path.relative(AGENT_DIR, fullPath)}`);
        }
      }
    }
  }
}

console.log('Starting minification of /.agent framework...');
traverseAndMinify(AGENT_DIR);
console.log('Done.');
