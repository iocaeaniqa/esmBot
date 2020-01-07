const client = require("../utils/client.js");

exports.run = async (message, args) => {
  if (!message.member.permission.has("manageMessages")) return `${message.author.mention}, you need to have the \`Manage Messages\` permission on this server to purge messages!`;
  if (!message.channel.guild.members.get(client.user.id).permission.has("manageMessages") && !message.channel.permissionsOf(client.user.id).has("manageMessages")) return `${message.author.mention}, I don't have the \`Manage Messages\` permission!`;
  if (args.length === 0 || !args[0].match(/^\d+$/)) return `${message.author.mention}, you need to provide the number of messages to purge!`;
  const numberOfMessages = parseInt(args[0]) + 1;
  await message.channel.purge(numberOfMessages);
  return `Successfully purged ${args[0]} messages.`;
};

exports.aliases = ["prune"];
exports.category = 2;
exports.help = "Purges messages in a channel";
exports.params = "[number]";